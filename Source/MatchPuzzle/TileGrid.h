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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ATileGrid();


	//그리드 크기 - width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridHeight;

	//타일 간의 배치 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	float TileSpacing;

	UPROPERTY()
	TArray<ATile*> TileArray;

	//타일을 생성한 ATile 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	TSubclassOf<ATile> TileClass;

	//그리드 초기화하는 함수
	void InitializeGrid();

	//특정 위치의 타일을 얻는 함수
	ATile* GetTileAt(int32 x, int32 y) const;

	//특정 위치의 타일을 설정하는 함수	
	void SetTileAt(int32 x, int32 y, ATile* tile);

	FName GenerateRandomTileType();

	TArray<ATile*> CheckForMatches();
	void SwapTiles(ATile* FirstTile, ATile* SecondTile);

	
	void RemoveMatchingTiles(const TArray<ATile*>& MatchingTiles);	
	void DropDownTiles();
	void RefillGrid();

	void ProcessMatchingLoop();
	
private:

	bool GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const;

	void FindMatchingTilesDFS(int32 StartX, int32 StartY, FName TargetTileType, TArray<ATile*>& OutMatchedTiles, TSet<ATile*>& Visited);
	void FindMatchingTilesBFS(int32 StartX, int32 StartY, FName TargetTileType, TArray<ATile*>& OutMatchedTiles);

	TArray<ATile*> CheckHorizontalMatches(int32 StartX, int32 StartY);
	TArray<ATile*> CheckVerticalMatches(int32 StartX, int32 StartY);
};
