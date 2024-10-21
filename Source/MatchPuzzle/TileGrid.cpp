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
		{FName("Cone"), FName("Cube"), FName("Cylinder"), FName("Sphere"),
		FName("Capsule"), FName("Pyramid")};

	for (int x = 0; x < GridWidth; ++x)
	{
		for(int y = 0; y < GridHeight; ++y)
		{
			//백그라운드에서 타일 타입을 결정하는 작업
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,[this,x,y,TileTypes]()
			{
				//타일 타입을 랜덤하게 결정 ( AsyncTask-> 비동기)
				FName RandomTileType = TileTypes[FMath::RandRange(0,TileTypes.Num() - 1)];

				AsyncTask(ENamedThreads::GameThread, [this,x,y,RandomTileType]()
				{
					//Tile 레퍼런스 Class 가 설정되지 않은 경우
					if(!TileClass)
					{
						UE_LOG(LogTemp, Error, TEXT("TileClass is not valid"));
						return;
					}

					//타일 생성 (GameThread *** 실행)
					FActorSpawnParameters SpawnParameters;
					ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass,SpawnParameters);

					if(NewTile)
					{
						//생성된 타일의 타일 설정
						NewTile->TileType = RandomTileType;

						//타일 종류에 따라서 외형 업데이트
						NewTile->UpdateTileAppearance();

						//타일을 타일그리드에 자식으로 부착
						NewTile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);

						//타일을 그리드에 배치
						FVector TileLocation = FVector(x* TileSpacing, y* TileSpacing, 0.0f);
						NewTile->SetActorRelativeLocation(TileLocation);

						SetTileAt(x,y,NewTile);						
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
