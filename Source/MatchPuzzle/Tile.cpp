// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"



// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

bool ATile::IsMatching(ATile* otherTile) const
{
	return TileType == otherTile->TileType;
}

void ATile::ProceesDataInParallerl()
{
	
	TArray<int32> DataArray;
	DataArray.Init(0,100);

	ParallelFor(DataArray.Num(), [&](int32 Index)
	{
		DataArray[Index] = Index*2;

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f
			   ,FColor::Red,FString::Printf(TEXT("Tile %d-%d"),Index,DataArray[Index]));
		}
	});

	UE_LOG(LogTemp, Warning, TEXT("ParallelForFinish"));
}


