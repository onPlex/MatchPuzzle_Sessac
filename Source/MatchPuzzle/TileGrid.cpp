// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"
#include "Tile.h"
#include "Async/Async.h"

void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
}

// Sets default values
ATileGrid::ATileGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridWidth = 8;
	GridHeight = 8;
	TileSpacing = 100.0f;
	TileArray.SetNum(GridWidth * GridHeight);
}

void ATileGrid::InitializeGrid()
{
	// 가능한 TileType 리스트
	TArray<FName> TileTypes =
	{ FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"),
	FName("Capsule"), FName("Pyramid") };
	/*
	  for (int32 x = 0; x < GridWidth; ++x)
	  {
		  for (int32 y = 0; y < GridHeight; ++y)
		  {
			  //타일 생성
			  FActorSpawnParameters SpawnParameters;
			  ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, SpawnParameters);

			  if (NewTile)
			  {
				  //타일의 그리드상의 위치
				  NewTile->TilePosition = FVector2D(x,y);

				  //생성된 타일의 타일 설정
				  NewTile->TileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];

				  //타일 종류에 따라서 외형 업데이트
				  NewTile->UpdateTileAppearance();

				  //타일을 타일그리드에 자식으로 부착
				  NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				  //타일을 그리드에 배치
				  FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.0f);
				  NewTile->SetActorRelativeLocation(TileLocation);

				  SetTileAt(x, y, NewTile);
			  }

		  }
	  }*/

	for (int x = 0; x < GridWidth; ++x)
	{
		for (int y = 0; y < GridHeight; ++y)
		{
			//백그라운드에서 타일 타입을 결정하는 작업
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, x, y, TileTypes]()
				{
					//타일 타입을 랜덤하게 결정 ( AsyncTask-> 비동기)
					FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];

					AsyncTask(ENamedThreads::GameThread, [this, x, y, RandomTileType]()
						{
							//Tile 레퍼런스 Class 가 설정되지 않은 경우
							if (!TileClass)
							{
								UE_LOG(LogTemp, Error, TEXT("TileClass is not valid"));
								return;
							}

							//타일 생성 (GameThread *** 실행)
							FActorSpawnParameters SpawnParameters;
							ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, SpawnParameters);

							if (NewTile)
							{
								//타일 그리드 참조
								NewTile->TileGrid = this;
								//타일의 그리드상의 위치
								NewTile->TilePosition = FVector2D(x, y);

								//생성된 타일의 타일 설정
								NewTile->TileType = RandomTileType;

								//타일 종류에 따라서 외형 업데이트
								NewTile->UpdateTileAppearance();

								//타일을 타일그리드에 자식으로 부착
								NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

								//타일을 그리드에 배치
								FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.0f);
								NewTile->SetActorRelativeLocation(TileLocation);

								SetTileAt(x, y, NewTile);
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("Failed to spawn tile"));
							}
						});
				});
		}
	}
}

ATile* ATileGrid::GetTileAt(int32 x, int32 y) const
{
	if (x < 0 || x >= GridWidth || y < 0 || y >= GridHeight)
	{
		return nullptr;
	}
	return TileArray[y * GridWidth + x];
}

void ATileGrid::SetTileAt(int32 x, int32 y, ATile* tile)
{
	if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight)
	{
		TileArray[y * GridWidth + x] = tile;
	}
}

TArray<ATile*> ATileGrid::CheckForMatches()
{
	//매칭이된 타일들
	TArray<ATile*> AllMatchedTiles;

	//Grid를 바둑판으로 for문으로 확인,
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{

			ATile* CurrentTile = GetTileAt(X, Y);
			if (!CurrentTile) continue;

			//매칭된 타일 찾기
			TArray<ATile*> HorizontalMatches = CheckHorizontalMatches(X, Y);

			if (HorizontalMatches.Num() >= 3)
			{
				AllMatchedTiles.Append(HorizontalMatches);
			}

		}
	}

	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{

			ATile* CurrentTile = GetTileAt(X, Y);
			if (!CurrentTile) continue;

			//매칭된 타일 찾기
			TArray<ATile*> VerticalMatches = CheckVerticalMatches(X, Y);

			if (VerticalMatches.Num() >= 3)
			{
				AllMatchedTiles.Append(VerticalMatches);
			}
		}
	}

	return AllMatchedTiles;
}

void ATileGrid::SwapTiles(ATile* FirstTile, ATile* SecondTile)
{
	int32 x1, y1, x2, y2;

	if (GetTileGridPosition(FirstTile, x1, y1) && GetTileGridPosition(SecondTile, x2, y2))
	{
		//타일 배열 업데이트
		SetTileAt(x1, y1, SecondTile);
		SetTileAt(x2, y2, FirstTile);

		//타일의 그리드 좌표 업데이트
		FVector2D TempPosition = FirstTile->TilePosition;
		FirstTile->TilePosition = SecondTile->TilePosition;
		SecondTile->TilePosition = TempPosition;

		//타일 위치를 스왑
		FVector TempLocation = FirstTile->GetActorLocation();
		FirstTile->SetActorLocation(SecondTile->GetActorLocation());
		FirstTile->SetActorLocation(TempLocation);
	}

}

void ATileGrid::RemoveMatchingTiles(const TArray<ATile*>& MatchingTiles)
{
	for (ATile* Tile : MatchingTiles)
	{
		//삭제할 타일이 존재
		if (Tile)
		{
			//삭제전, 그리드상에서의 위치 null +삭제
			int32 X, Y;
			if (GetTileGridPosition(Tile, X, Y))
			{
				SetTileAt(X, Y, nullptr);

				Tile->Destroy();
			}
		}
	}


	/*
	* 점수 증가
	*/

	DropDownTiles();

	//이후 매칭 루프처리
	ProcessMatchingLoop();
}

void ATileGrid::DropDownTiles()
{
	for (int32 X = 0; X < GridWidth; X++)
	{
		for (int32 Y = GridHeight - 1; Y >= 0; --Y) //아래에서 위로 확인
		{
			if (!GetTileAt(X, Y)) //빈 칸인 경우
			{
				for (int32 AboveY = Y - 1; AboveY >= 0; --AboveY)
				{
					ATile* AboveTile = GetTileAt(X, AboveY);
					if (AboveTile)
					{
						//타일 배열 업데이트

						SetTileAt(X, Y, AboveTile);
						SetTileAt(X, AboveY, nullptr);

						//타일의 그리드 좌표갱신
						AboveTile->TilePosition = FVector2D(X, Y);

						// 타일을 이동 (상대적 위치로)
						FVector RelativeLocation = FVector(X * TileSpacing, Y * TileSpacing, 0);
						AboveTile->SetActorRelativeLocation(RelativeLocation);

						break;
					}

				}
			}
		}
	}


	RefillGrid();
}

TArray<ATile*> ATileGrid::CheckHorizontalMatches(int32 StartX, int32 StartY)
{
	TArray<ATile*> HorizontalMatches;
	ATile* StartTile = GetTileAt(StartX, StartY);

	if (!StartTile)return HorizontalMatches;

	HorizontalMatches.Add(StartTile);

	//가로 2칸까지 같은 타일이 있는지확인
	for (int32 x = StartX + 1; x < GridWidth; ++x)
	{
		ATile* NewTile = GetTileAt(x, StartY);
		if (NewTile && NewTile->TileType == StartTile->TileType)
		{
			HorizontalMatches.Add(NewTile);
		}
		else
		{
			break;
		}
	}
	return HorizontalMatches;
}

TArray<ATile*> ATileGrid::CheckVerticalMatches(int32 StartX, int32 StartY)
{
	TArray<ATile*> VerticallMatches;
	ATile* StartTile = GetTileAt(StartX, StartY);

	if (!StartTile)return VerticallMatches;

	VerticallMatches.Add(StartTile);

	//가로 2칸까지 같은 타일이 있는지확인
	for (int32 Y = StartY + 1; Y < GridHeight; ++Y)
	{
		ATile* NewTile = GetTileAt(StartX, Y);
		if (NewTile && NewTile->TileType == StartTile->TileType)
		{
			VerticallMatches.Add(NewTile);
		}
		else
		{
			break;
		}
	}
	return VerticallMatches;
}

bool ATileGrid::GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const
{

	for (int32 i = 0; i < TileArray.Num(); ++i)
	{
		if (TileArray[i] == Tile)
		{
			OutX = i % GridWidth; // X 좌표 계산
			OutY = i / GridWidth; // Y 좌표 계산 
			return true;
		}
	}

	return false;
}

void ATileGrid::RefillGrid()
{
	for (int32 x = 0; x < GridWidth; ++x)
	{
		for (int32 y = 0; y < GridHeight; ++y)
		{
			//x,y 좌표위치 그리드에 타일이 없다면,
			if (!GetTileAt(x, y))
			{
				//새로운 타일 생성
				ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass);

				if (NewTile)
				{
					//타일 그리드 참조
					NewTile->TileGrid = this;
					//타일의 그리드상의 위치
					NewTile->TilePosition = FVector2D(x, y);

					//생성된 타일의 타일 설정
					NewTile->TileType = GenerateRandomTileType();

					//타일 종류에 따라서 외형 업데이트
					NewTile->UpdateTileAppearance();

					//타일을 타일그리드에 자식으로 부착
					NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

					//타일을 그리드에 배치
					FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.0f);
					NewTile->SetActorRelativeLocation(TileLocation);

					SetTileAt(x, y, NewTile);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to spawn tile"));
				}
			}
		}
	}

	// 또 다른 매칭이 있는지 확인
	ProcessMatchingLoop();
}

void ATileGrid::ProcessMatchingLoop()
{
	//매칭이 있는지 확인 
	TArray<ATile*> Matingtiles = CheckForMatches();


	//매칭된 타일이 존재
	if (Matingtiles.Num() > 0)
	{
		//삭제 -> 당기고 -> 빈자리에 생성
		//매칭된 타일이 있을 경우 

		// 매칭된 타일이 있을 경우 삭제 
		RemoveMatchingTiles(Matingtiles);

		//타일을 빈공간으로 이동
		DropDownTiles();

		//빈 공간에 새 타일 생성
		RefillGrid();


		//모든 작업이 끝난 후에도 ~  매칭확인
		ProcessMatchingLoop(); // 재귀 호출 
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No More Matches"));
	}

}


FName ATileGrid::GenerateRandomTileType()
{
	// 가능한 TileType 리스트
	TArray<FName> TileTypes =
	{ FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"),
	FName("Capsule"), FName("Pyramid") };

	return TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
}
