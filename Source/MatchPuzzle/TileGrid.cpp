#include "TileGrid.h"

#include "GI_Puzzle.h"
#include "Match3GameMode.h"
#include "Tile.h"
#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"

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
	TArray<FName> TileTypes = {
		FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"), FName("Capsule"), FName("Pyramid")
	};

	for (int x = 0; x < GridWidth; ++x)
	{
		for (int y = 0; y < GridHeight; ++y)
		{
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, x, y, TileTypes]()
			{
				FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];

				AsyncTask(ENamedThreads::GameThread, [this, x, y, RandomTileType]()
				{
					if (!TileClass)
					{
						UE_LOG(LogTemp, Error, TEXT("TileClass is not valid"));
						return;
					}

					FActorSpawnParameters SpawnParameters;
					ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, SpawnParameters);

					if (NewTile)
					{
						NewTile->TileGrid = this;
						NewTile->TilePosition = FVector2D(x, y);
						NewTile->TileType = RandomTileType;
						NewTile->UpdateTileAppearance();
						NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
						FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.0f);
						NewTile->SetActorRelativeLocation(TileLocation);
						SetTileAt(x, y, NewTile);
					}
				});
			});
		}
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

		// 타일 위치를 스왑 (물리적 위치도 스왑)
		FVector TempLocation = FirstTile->GetActorLocation();
		FirstTile->SetActorLocation(SecondTile->GetActorLocation());
		SecondTile->SetActorLocation(TempLocation);

		// 디버그: 타일 위치와 배열 확인
		UE_LOG(LogTemp, Warning, TEXT("Tile 1 Pos: %s, Tile 2 Pos: %s"), *FirstTile->TilePosition.ToString(), *SecondTile->TilePosition.ToString());
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

FName ATileGrid::GenerateRandomTileType()
{
	// 가능한 TileType 리스트
	TArray<FName> TileTypes =
	{
		FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"),
		FName("Capsule"), FName("Pyramid")
	};

	//타일 타입을 랜덤하게 결정 ( AsyncTask-> 비동기)
	FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
	return RandomTileType;
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

void ATileGrid::RemoveMatchingTiles(const TArray<ATile*>& MatchingTiles)
{
	// GameInstance 가져오기
	UGI_Puzzle* GameInstance = Cast<UGI_Puzzle>(GetWorld()->GetGameInstance());

	for (ATile* Tile : MatchingTiles)
	{
		if (Tile)
		{
			int32 X, Y;
			if (GetTileGridPosition(Tile, X, Y))
			{
				SetTileAt(X, Y, nullptr); // 그리드에서 타일을 삭제 (먼저 배열에서 제거)                
				Tile->Destroy(); // 타일 객체 삭제
			}

			// 점수 추가 (매칭된 타일의 수에 따라 점수 부여)
			if (GameInstance)
			{
				GameInstance->AddScore(10); // 예시로 10점 추가
			}
		}
	}
	// 삭제 후 빈 공간을 채우기
	DropDownTiles(); // 타일을 아래로 이동

	ProcessMatchingLoop();
}

void ATileGrid::DropDownTiles()
{
	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = GridHeight - 1; Y >= 0; --Y) // 아래에서 위로 확인
		{
			if (!GetTileAt(X, Y)) // 빈 칸일 경우
			{
				for (int32 AboveY = Y - 1; AboveY >= 0; --AboveY)
				{
					ATile* AboveTile = GetTileAt(X, AboveY);
					if (AboveTile)
					{
						// 타일 배열에서 먼저 업데이트
						SetTileAt(X, Y, AboveTile);
						SetTileAt(X, AboveY, nullptr);

						// 타일 위치 업데이트
						FVector RelativeLocation = FVector(X * TileSpacing, Y * TileSpacing, 0.0f);
						AboveTile->SetActorRelativeLocation(RelativeLocation);

						// 타일의 그리드 좌표도 업데이트
						AboveTile->TilePosition = FVector2D(X, Y);

						break;
					}
				}
			}
		}
	}

	// 빈 칸을 새로운 타일로 채우기
	RefillGrid();
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
					NewTile->TileGrid = this;
					NewTile->TilePosition = FVector2D(X, Y);
					NewTile->TileType = GenerateRandomTileType();
					NewTile->UpdateTileAppearance();

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

	ProcessMatchingLoop();
}

void ATileGrid::ProcessMatchingLoop()
{
	TArray<ATile*> MatcingTiles = CheckForMatches();

	if(MatcingTiles.Num() > 0 )
	{
		RemoveMatchingTiles(MatcingTiles);

		DropDownTiles();

		RefillGrid();

		ProcessMatchingLoop();
	}
	else
	{
		UE_LOG(LogTemp,Display, TEXT("No More Match"));
	}
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

void ATileGrid::FindMatchingTilesDFS(int32 StartX, int32 StartY, FName TargetTileType, TArray<ATile*>& OutMatchedTiles,
                                     TSet<ATile*>& Visited)
{
	// 보드 경계 검사
	if (StartX < 0 || StartX >= GridWidth || StartY < 0 || StartY >= GridHeight)
		return;

	// 특정 위치의 타일을 가져옴
	ATile* CurrentTile = GetTileAt(StartX, StartY);
	if (!CurrentTile || Visited.Contains(CurrentTile))
		return;

	// 타일이 매칭되지 않으면 중단
	if (CurrentTile->TileType != TargetTileType)
		return;

	// 방문 처리 및 매칭된 타일에 추가
	Visited.Add(CurrentTile);
	OutMatchedTiles.Add(CurrentTile);

	// 상하좌우 인접한 타일을 재귀적으로 탐색 (DFS)
	FindMatchingTilesDFS(StartX + 1, StartY, TargetTileType, OutMatchedTiles, Visited); // 오른쪽
	FindMatchingTilesDFS(StartX - 1, StartY, TargetTileType, OutMatchedTiles, Visited); // 왼쪽
	FindMatchingTilesDFS(StartX, StartY + 1, TargetTileType, OutMatchedTiles, Visited); // 아래
	FindMatchingTilesDFS(StartX, StartY - 1, TargetTileType, OutMatchedTiles, Visited); // 위
}

void ATileGrid::FindMatchingTilesBFS(int32 StartX, int32 StartY, FName TargetTileType, TArray<ATile*>& OutMatchedTiles)
{
	TSet<ATile*> Visited;
	TQueue<FIntPoint> Queue;

	Queue.Enqueue(FIntPoint(StartX, StartY));
	Visited.Add(GetTileAt(StartX, StartY));

	while (!Queue.IsEmpty())
	{
		FIntPoint Current;
		Queue.Dequeue(Current);
		ATile* CurrentTile = GetTileAt(Current.X, Current.Y);

		OutMatchedTiles.Add(CurrentTile);

		// 상하좌우 탐색
		TArray<FIntPoint> Neighbors = {
			FIntPoint(Current.X + 1, Current.Y),
			FIntPoint(Current.X - 1, Current.Y),
			FIntPoint(Current.X, Current.Y + 1),
			FIntPoint(Current.X, Current.Y - 1)
		};

		for (FIntPoint Neighbor : Neighbors)
		{
			int32 nx = Neighbor.X;
			int32 ny = Neighbor.Y;

			if (nx >= 0 && nx < GridWidth && ny >= 0 && ny < GridHeight)
			{
				ATile* NeighborTile = GetTileAt(nx, ny);
				if (!Visited.Contains(NeighborTile) && NeighborTile->TileType == TargetTileType)
				{
					Queue.Enqueue(FIntPoint(nx, ny));
					Visited.Add(NeighborTile);
				}
			}
		}
	}
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
