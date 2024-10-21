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
