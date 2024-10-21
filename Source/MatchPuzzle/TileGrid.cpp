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
	TileSpacing = 100.0f;  // 기본 배치 간격 설정
	TileArray.SetNum(GridWidth * GridHeight);
}

void ATileGrid::InitializeGrid()
{

	TArray<FName> TileTypes = { FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"), FName("Capsule"), FName("Pyramid") };

	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			// 타일 생성
			FActorSpawnParameters SpawnParams;
			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, SpawnParams); 

			if (NewTile)
			{
				// 타일 설정
				NewTile->TileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
				NewTile->UpdateTileAppearance();
				NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				// 타일 위치 설정
				FVector RelativeLocation = FVector(X * TileSpacing, Y * TileSpacing, 0.0f);
				NewTile->SetActorRelativeLocation(RelativeLocation);

				// 그리드에 타일 저장
				SetTileAt(X, Y, NewTile);

				// 타일 생성과 배치 정보 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("Tile created at [%d, %d] with relative location %s"), X, Y, *RelativeLocation.ToString());
			}
		}
	}
	/*
	 // 가능한 TileType 리스트
    TArray<FName> TileTypes = { FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"), FName("Capsule"), FName("Pyramid") };

    for (int32 x = 0; x < GridWidth; ++x)
    {
        for (int32 y = 0; y < GridHeight; ++y)
        {
            // 백그라운드에서 타일 타입을 결정하는 작업을 비동기로 수행
            AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, x, y, TileTypes]()
            {
                // 타일 타입을 랜덤하게 결정 (비동기 작업)
                FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];

                // 게임 스레드에서 타일 생성과 외형 설정을 처리
                AsyncTask(ENamedThreads::GameThread, [this, x, y, RandomTileType]()
                {
                    if (!TileClass)  // TileClass가 설정되지 않은 경우 로그를 출력
                    {
                        UE_LOG(LogTemp, Error, TEXT("TileClass is not set in TileGrid"));
                        return;
                    }

                    // 타일 생성 (게임 스레드에서 실행)
                    FActorSpawnParameters SpawnParams;
                    ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, SpawnParams);  // Blueprint 타일 클래스를 사용

                	if (NewTile)
                	{
						// 생성된 타일의 타입 설정
						NewTile->TileType = RandomTileType;

						// 타일 외형 업데이트
						NewTile->UpdateTileAppearance();

						// 타일을 TileGrid의 자식으로 부착
						NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

						// 상대적인 위치로 타일을 배치
						FVector RelativeLocation = FVector(x * TileSpacing, y * TileSpacing, 0.0f);
						NewTile->SetActorRelativeLocation(RelativeLocation);

						// 그리드에 타일 저장
						SetTileAt(x, y, NewTile);

						// 디버그 출력
						UE_LOG(LogTemp, Warning, TEXT("Tile created at [%d, %d] with type %s at relative location %s"), x, y, *RandomTileType.ToString(), *RelativeLocation.ToString());
					}
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to spawn tile at [%d, %d]"), x, y);
                    }
                });
            });
        }
    }*/
}

ATile* ATileGrid::GetTileAt(int32 x, int32 y) const
{
	if (x < 0 || x >= GridWidth || y < 0 || y >= GridHeight)
	{
		return nullptr;
	}
	return TileArray[y * GridWidth + x];
}

void ATileGrid::SetTileAt(int32 X, int32 Y, ATile* Tile)
{
	if (X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight)
    {
        int32 Index = X + (Y * GridWidth);
        TileArray[Index] = Tile;

        // 좌표가 올바르게 설정되었는지 확인하기 위해 로그 추가
        UE_LOG(LogTemp, Warning, TEXT("Tile set at [%d, %d], Array Index: %d"), X, Y, Index);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid tile coordinates: [%d, %d]"), X, Y);
    }
}

// 타일의 인접 여부를 확인하는 함수 (가로 또는 세로 인접 여부)
bool ATileGrid::AreTilesAdjacent(ATile* FirstTile, ATile* SecondTile)
{
	int32 FirstX, FirstY, SecondX, SecondY;

	// 각 타일의 그리드 좌표를 얻음
	if (GetTileGridPosition(FirstTile, FirstX, FirstY) && GetTileGridPosition(SecondTile, SecondX, SecondY))
	{
		// 로그 출력 (좌표 확인)
		UE_LOG(LogTemp, Warning, TEXT("First Tile [%d, %d], Second Tile [%d, %d]"), FirstX, FirstY, SecondX, SecondY);

		// 가로 또는 세로로 인접한 경우 (대각선은 제외)
		bool bIsAdjacent = (FMath::Abs(FirstX - SecondX) == 1 && FirstY == SecondY) ||
						   (FMath::Abs(FirstY - SecondY) == 1 && FirstX == SecondX);
        
		// 인접한 경우와 아닌 경우 로그 출력
		if (bIsAdjacent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tiles are adjacent."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tiles are not adjacent."));
		}

		return bIsAdjacent;
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not get tile positions."));
	return false;
}

// 타일의 그리드 좌표를 얻는 함수
bool ATileGrid::GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const
{	
	for (int32 Index = 0; Index < TileArray.Num(); ++Index)
	{
		if (TileArray[Index] == Tile)
		{
			OutX = Index % GridWidth;  // X 좌표 계산
			OutY = Index / GridWidth;  // Y 좌표 계산

			// 좌표가 제대로 계산되는지 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("Found Tile at Array Index: %d, Grid Position: [%d, %d]"), Index, OutX, OutY);

			return true;
		}
	}

	// 타일을 찾지 못한 경우
	UE_LOG(LogTemp, Error, TEXT("Tile not found in TileArray."));
	return false;
}