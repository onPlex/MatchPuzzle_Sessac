// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3GameMode.h"

#include "MyGameInstance.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

void AMatch3GameMode::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(
		UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstance)
	{
		MyGameInstance->ResetGameState();
	}

	//타일그리도 초기화
	//타일그리드가 월드에 존재시, 생성은 불필요
	ATileGrid* TileGrid = GetWorld()->SpawnActor<ATileGrid>(ATileGrid::StaticClass());
	if (TileGrid)
	{
		TileGrid->InitializeGrid();
	}
}
