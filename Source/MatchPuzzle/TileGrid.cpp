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
	{
		FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"),
		FName("Capsule"), FName("Pyramid")
	};
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
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this,x,y,TileTypes]()
			{
				//타일 타입을 랜덤하게 결정 ( AsyncTask-> 비동기)
				FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];

				AsyncTask(ENamedThreads::GameThread, [this,x,y,RandomTileType]()
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
						// 타일 속성 설정
						NewTile->TileGrid = this; // 타일 그리드 참조
						NewTile->TilePosition = FVector2D(x, y); // 그리드 좌표
						NewTile->TileType = RandomTileType; // 랜덤 타일 타입
						NewTile->UpdateTileAppearance(); // 외형 업데이트

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

void ATileGrid::SwapTiles(ATile* FirstTile, ATile* SecondTile)
{
	int32 X1, Y1, X2, Y2;
	if (GetTileGridPosition(FirstTile, X1, Y1) && GetTileGridPosition(SecondTile, X2, Y2))
	{
		// 타일 배열 업데이트
		SetTileAt(X1, Y1, SecondTile);
		SetTileAt(X2, Y2, FirstTile);

		// 타일의 그리드 좌표도 업데이트
		FVector2D TempPosition = FirstTile->TilePosition;
		FirstTile->TilePosition = SecondTile->TilePosition;
		SecondTile->TilePosition = TempPosition;

		// 타일 위치를 스왑
		FVector TempLocation = FirstTile->GetActorLocation();
		FirstTile->SetActorLocation(SecondTile->GetActorLocation());
		SecondTile->SetActorLocation(TempLocation);
	}
}

TArray<ATile*> ATileGrid::CheckForMatches()
{
	TArray<ATile*> AllMatchedTiles;

	// 가로 매칭 확인
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			ATile* CurrentTile = GetTileAt(X, Y);
			if (!CurrentTile) continue;

			// 가로 매칭된 타일 찾기
			TArray<ATile*> HorizontalMatches = CheckHorizontalMatches(X, Y);
			if (HorizontalMatches.Num() >= 3)
			{
				AllMatchedTiles.Append(HorizontalMatches);
			}
		}
	}

	// 세로 매칭 확인
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			ATile* CurrentTile = GetTileAt(X, Y);
			if (!CurrentTile) continue;

			// 세로 매칭된 타일 찾기
			TArray<ATile*> VerticalMatches = CheckVerticalMatches(X, Y);
			if (VerticalMatches.Num() >= 3)
			{
				AllMatchedTiles.Append(VerticalMatches);
			}
		}
	}

	return AllMatchedTiles;
}

TArray<ATile*> ATileGrid::CheckHorizontalMatches(int32 StartX, int32 StartY)
{
	TArray<ATile*> HorizontalMatches;
	ATile* StartTile = GetTileAt(StartX, StartY);

	if (!StartTile) return HorizontalMatches;

	HorizontalMatches.Add(StartTile);

	// 오른쪽으로 2칸까지 같은 타일이 있는지 확인
	for (int32 X = StartX + 1; X < GridWidth; ++X)
	{
		ATile* NextTile = GetTileAt(X, StartY);
		if (NextTile && NextTile->TileType == StartTile->TileType)
		{
			HorizontalMatches.Add(NextTile);
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
	TArray<ATile*> VerticalMatches;
	ATile* StartTile = GetTileAt(StartX, StartY);

	if (!StartTile) return VerticalMatches;

	VerticalMatches.Add(StartTile);

	// 아래로 2칸까지 같은 타일이 있는지 확인
	for (int32 Y = StartY + 1; Y < GridHeight; ++Y)
	{
		ATile* NextTile = GetTileAt(StartX, Y);
		if (NextTile && NextTile->TileType == StartTile->TileType)
		{
			VerticalMatches.Add(NextTile);
		}
		else
		{
			break;
		}
	}

	return VerticalMatches;
}

bool ATileGrid::GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const
{
	for (int32 Index = 0; Index < TileArray.Num(); ++Index)
	{
		if (TileArray[Index] == Tile)
		{
			OutX = Index % GridWidth; // X 좌표 계산
			OutY = Index / GridWidth; // Y 좌표 계산
			return true;
		}
	}
	// 타일을 찾지 못한 경우
	return false;
}

void ATileGrid::DropDownTiles()
{
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = GridHeight - 1; Y >= 0; --Y)  // 아래에서 위로 확인
		{
			if (!GetTileAt(X, Y))  // 빈 칸일 경우
			{
				for (int32 AboveY = Y - 1; AboveY >= 0; --AboveY)
				{
					ATile* AboveTile = GetTileAt(X, AboveY);
					if (AboveTile)
					{
						// 타일 배열 업데이트
						SetTileAt(X, Y, AboveTile);
						SetTileAt(X, AboveY, nullptr);

						// 타일의 그리드 좌표 갱신
						AboveTile->TilePosition = FVector2D(X, Y);

						// 타일을 이동 (상대적 위치로)
						FVector RelativeLocation = FVector(X * TileSpacing, Y * TileSpacing, 0.0f);
						AboveTile->SetActorRelativeLocation(RelativeLocation);

						break;
					}
				}
			}
		}
	}

	// 빈 칸을 새로운 타일로 채우기
	RefillGrid();
}

// 랜덤한 타일 타입을 생성하는 함수
FName ATileGrid::GenerateRandomTileType()
{
	TArray<FName> TileTypes = {
		FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"), FName("Capsule"), FName("Pyramid")
	};
	return TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
}

void ATileGrid::ProcessMatchingLoop()
{
	// 매칭이 있는지 확인
	TArray<ATile*> MatchingTiles = CheckForMatches();

	if (MatchingTiles.Num() > 0)
	{
		// 매칭된 타일이 있을 경우 삭제
		RemoveMatchingTiles(MatchingTiles);

		// 타일을 빈 공간으로 이동
		DropDownTiles();

		// 빈 공간에 새로운 타일을 채우기
		RefillGrid();

		// 모든 작업이 끝난 후 다시 매칭을 확인하기 위해 재귀 호출 (매칭이 더 이상 없을 때까지 반복)
		ProcessMatchingLoop();  // 재귀 호출
	}
	else
	{
		// 더 이상 매칭이 없으면 루프 종료
		UE_LOG(LogTemp, Display, TEXT("No more matches, puzzle stabilized."));
	}
}

void ATileGrid::RefillGrid()
{
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			if (!GetTileAt(X, Y))
			{
				// 새로운 타일 생성
				ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass);

				if (NewTile)
				{
					// 타일의 그리드 좌표 설정
					NewTile->TilePosition = FVector2D(X, Y);
					NewTile->TileType = GenerateRandomTileType();
					NewTile->UpdateTileAppearance();
					NewTile->TileGrid = this;

					// 타일을 TileGrid의 자식으로 설정
					NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

					// 상대적 위치 설정
					FVector RelativeLocation = FVector(X * TileSpacing, Y * TileSpacing, 0.0f);
					NewTile->SetActorRelativeLocation(RelativeLocation);

					// 그리드에 새로운 타일 설정 (TileArray 업데이트)
					SetTileAt(X, Y, NewTile);
				}
			}
		}
	}

	// 새로운 타일이 생성된 후 매칭 루프 처리
	ProcessMatchingLoop();  // 매칭 루프 시작
}

void ATileGrid::RemoveMatchingTiles(const TArray<ATile*>& MatchingTiles)
{
	for (ATile* Tile : MatchingTiles)
	{
		if (Tile)
		{
			int32 X, Y;
			if (GetTileGridPosition(Tile, X, Y))
			{
				SetTileAt(X, Y, nullptr); // 그리드에서 타일 삭제
				Tile->Destroy(); // 타일 객체 삭제
			}
		}
	}

	/*
	*  // 매칭된 타일 수에 따라 점수 추가
	AddScore(NumMatchedTiles * 100);  // 예시: 타일 하나당 100점
	 */
	
	// 빈 공간을 채우기 위해 타일을 아래로 드랍
	DropDownTiles();

	// 이후 매칭 루프 처리
	ProcessMatchingLoop();  // 매칭 루프 시작
}
