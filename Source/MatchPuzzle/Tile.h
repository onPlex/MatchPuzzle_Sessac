#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"
UCLASS()
class MATCHPUZZLE_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// 타일이 선택되었는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	bool bIsSelected;

	// 타일을 선택 또는 해제하는 함수
	void SetSelected(bool bSelected);

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 타일의 시각적 상태를 업데이트하는 함수
	void UpdateAppearance();

public:
	// 타일의 타입을 정의
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	FName TileType;

	// 타일의 외형을 위한 Static Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile Properties")
	UStaticMeshComponent* TileMesh;

	// 타일의 StaticMesh 설정을 위한 TSubclassOf
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	TMap<FName, UStaticMesh*> TileMeshes;

	// 타일 매칭 확인 함수
	bool IsMatching(ATile* OtherTile) const;

	// 타일의 외형을 TileType에 따라 설정하는 함수
	void UpdateTileAppearance();

	// 병렬 처리를 테스트하는 함수
	void ProceesDataInParallel();


	// 타일의 2D 좌표 (그리드 상의 위치)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	FVector2D TilePosition;
	// 다른 타일과 인접 여부를 확인하는 함수
	bool IsAdjacentTo(ATile* OtherTile) const;
	// 타일의 위치를 변경할 때 그리드 상의 위치를 업데이트하는 함수
	void UpdateTilePosition(const FVector2D& NewPosition);
};
