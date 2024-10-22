// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3GameMode.h"
#include "GameStateSubject.h"
#include "MyGameInstance.h"
#include "TileGrid.h"
#include "GameWidgetObserver.h"
#include "Kismet/GameplayStatics.h"

void AMatch3GameMode::BeginPlay()
{
	Super::BeginPlay();

	//UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(
	//	UGameplayStatics::GetGameInstance(GetWorld()));
//
	//if (MyGameInstance)
	//{
	//	MyGameInstance->ResetGameState();
	//}
//
	////타일그리도 초기화
	////타일그리드가 월드에 존재시, 생성은 불필요
	//ATileGrid* TileGrid = GetWorld()->SpawnActor<ATileGrid>(ATileGrid::StaticClass());
	//if (TileGrid)
	//{
	//	TileGrid->InitializeGrid();
	//}
//
    ////Observer 주체 생성 
	//UGameStateSubject* ObseverGameState = NewObject<UGameStateSubject>();
//
	//UGameWidgetObserver* ScoreWidget 
	//= CreateWidget<UGameWidgetObserver>(GetWorld(), LoadClass<UGameWidgetObserver>(nullptr,TEXT("/Game/UI/ScoreWidget")));
	//if (ScoreWidget)
	//{
	//	ScoreWidget->AddToViewport();
//
	//	//위젯을 옵저버로 등록 
	//	ObseverGameState->RegisterObserver(ScoreWidget);
	//}
}
