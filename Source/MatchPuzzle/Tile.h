// Fill out your copyright notice in the Description page of Project Settings.

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


	//타일이 선택되었는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bIsSelected;

	//타일을 선택 또는 해제하는 함수
	void SetSelected(bool bSelected);
	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //타일이 선택되면 시각적으로 상태가 업데이트
	void UpdateAppearance();
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	FName TileType;

	//타일 매칭 확인 함수
	bool IsMatching(ATile* otherTile) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Properties")
	UStaticMeshComponent* TileMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	TMap<FName, UStaticMesh*> TileMeshes;


	//타일 매칭 확인 함수
	bool IsMatcing(ATile* OtherTile) const;

	//타일의 외형을 TileType에 따라 설정하는 함수
	void UpdateTileAppearance();
	
	void ProceesDataInParallerl();

};
