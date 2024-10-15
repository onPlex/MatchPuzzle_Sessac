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
	TileArray.SetNum(GridWidth * GridHeight);
}

void ATileGrid::InitializeGrid()
{
	////for문을 돌며, 타일 그리드 그리기
	//for (int32 x = 0; x < GridWidth; ++x)
	//{
	//	for (int32 y = 0; y < GridHeight; ++y)
	//	{
	//		//타일 생성
	//		ATile* NewTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass());
	//		SetTileAt(x, y, NewTile);
	//	}
	//}

	//병렬로 처리를 할는 For ( ParallerFor)
	//ParallelFor(GridWidth * GridWidth,[this](int32 Index)
	//{
	//	int32 X = Index % GridWidth;
	//	int32 Y = Index / GridWidth;
	//
	//	// 타일 (ATile) 생성하고 -> 배열에 저장
	//	ATile* NewTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass());
	// 	SetTileAt(X, Y, NewTile);
	//});
	//UE_LOG()


	for (int32 x = 0; x < GridWidth; ++x)
	{
		for (int32 y = 0; y < GridHeight; ++y)
		{
			//비동기작업
			AsyncTask(ENamedThreads::GameThread, [this,x,y]()
			{
				ATile* NewTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass());
				SetTileAt(x, y, NewTile);
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
