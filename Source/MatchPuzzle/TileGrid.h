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

	// 타일 간의 배치 간격 (기본값: 100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	float TileSpacing;
	UPROPERTY()
	TArray<ATile*> TileArray;

	// 타일을 생성할 Blueprint 클래스를 선택할 수 있도록 TSubclassOf 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	TSubclassOf<ATile> TileClass;  // TileClass는 월드에 배치된 TileGrid에서 설정할 수 있음
	
	//그리드 초기화하는 함수
	void InitializeGrid();

	//특정 위치의 타일을 얻는 함수
	ATile* GetTileAt(int32 x, int32 y) const;

	//특정 위치의 타일을 설정하는 함수	
	void SetTileAt(int32 X, int32 Y, ATile* Tile);

	// 타일의 인접 여부를 확인하는 함수
	bool AreTilesAdjacent(ATile* FirstTile, ATile* SecondTile);

	// 타일의 그리드 좌표를 얻는 함수
	bool GetTileGridPosition(ATile* Tile, int32& OutX, int32& OutY) const;
};
