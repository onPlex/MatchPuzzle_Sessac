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

	// 타일을 교환하는 함수
	void SwapTiles(ATile* FirstTile, ATile* SecondTile);

	// 매칭된 타일을 확인하는 함수 (가로 및 세로 매칭 확인)
	TArray<ATile*> CheckForMatches();

	// 그리드 내에서 타일의 X, Y 좌표를 가져오는 함수
	bool GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const;

	// 타일을 아래로 드랍하는 함수 (빈 칸을 채움)
	void DropDownTiles();

	// 빈 공간을 새로운 타일로 채우는 함수
	void RefillGrid();

	// 매칭된 타일을 제거하는 함수
	void RemoveMatchingTiles(const TArray<ATile*>& MatchingTiles);
	
private:
	// 가로로 매칭된 타일을 찾는 함수
	TArray<ATile*> CheckHorizontalMatches(int32 StartX, int32 StartY);

	// 세로로 매칭된 타일을 찾는 함수
	TArray<ATile*> CheckVerticalMatches(int32 StartX, int32 StartY);

	// 랜덤한 타일 타입을 생성하는 함수
	FName GenerateRandomTileType();
};
