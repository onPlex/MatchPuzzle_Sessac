// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TileGamePlayerController.generated.h"

/**
 * 
 */
class ATile;
class ATileGrid;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MATCHPUZZLE_API ATileGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATileGamePlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	//첫 번째 선택 타일 
	TWeakObjectPtr<ATile> FirstSelecetTile;
	//두 번째 선택 타일 
	TWeakObjectPtr<ATile> SecondSelecetTile;

	//TileGrid  참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	ATileGrid* TileGrid;

	//Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* SelectInputAction;

	//타일을 선택하는 함수
	void SelectTile(const FInputActionValue& Value);

	//두 개의 타일을 선택하고 처리하는 함수
	void ProcessSelectTiles();
};
