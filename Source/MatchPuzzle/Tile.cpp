// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include"Components/StaticMeshComponent.h"


// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Static 메쉬 컴포넌트 생성
	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	RootComponent = TileMeshComponent;

	TileType = FName("Default");
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	UpdateTileAppearance();
}

bool ATile::IsMatching(ATile* otherTile) const
{
	return TileType == otherTile->TileType;
}

bool ATile::IsMatcing(ATile* OtherTile) const
{
	// 같은 타일인지 여부 -> OtherTile 이랑 같은 타입인가 ?
	return TileType == OtherTile->TileType;
}

void ATile::UpdateTileAppearance()
{
	//TileMeshes Map에 TileType에 해당하는 키값이 존재하는가 ?
	if (TileMeshes.Contains(TileType))
	{
		TileMeshComponent->SetStaticMesh(TileMeshes[TileType]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attempting to update tile appearance"));
	}
}

void ATile::ProceesDataInParallerl()
{
	TArray<int32> DataArray;
	DataArray.Init(0, 100);

	ParallelFor(DataArray.Num(), [&](int32 Index)
	{
		DataArray[Index] = Index * 2;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f
			                                 , FColor::Red,
			                                 FString::Printf(TEXT("Tile %d-%d"), Index, DataArray[Index]));
		}
	});
	UE_LOG(LogTemp, Warning, TEXT("ParallelForFinish"));
}
