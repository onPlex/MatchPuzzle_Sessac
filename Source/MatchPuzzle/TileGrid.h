// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileGrid.generated.h"

class ATile;

UCLASS()
class MATCHPUZZLE_API ATileGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATileGrid();


	//그리드 크기 - width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridHeight;

	UPROPERTY()
	TArray<ATile*> TileArray;

	//그리드 초기화하는 함수
	void InitializeGrid();

	//특정 위치의 타일을 얻는 함수
	ATile* GetTileAt(int32 x, int32 y) const;

	//특정 위치의 타일을 설정하는 함수
	
	void SetTileAt(int32 x, int32 y, ATile* tile);
};
