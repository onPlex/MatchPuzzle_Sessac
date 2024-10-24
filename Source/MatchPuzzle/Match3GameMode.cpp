// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3GameMode.h"
#include "GameStateSubject.h"
#include "GI_Puzzle.h"
#include "GameWidgetObserver.h"
#include "Kismet/GameplayStatics.h"

void AMatch3GameMode::BeginPlay()
{
	Super::BeginPlay();

	
	UGI_Puzzle* MyGameInstance = Cast<UGI_Puzzle>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		MyGameInstance->ResetGameState();
	}

	// Observer 주체 생성
	UGameStateSubject* ObserverGameState = NewObject<UGameStateSubject>();

	// GameInstance에 Observer 주체 설정
	if (MyGameInstance)
	{
		MyGameInstance->SetGameStateSubject(ObserverGameState);
	}

	if (MainWidgetClass)
	{
		UGameWidgetObserver* ScoreWidget = CreateWidget<UGameWidgetObserver>(GetWorld(), MainWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			ObserverGameState->RegisterObserver(ScoreWidget);

			// 디버그 로그 추가
			UE_LOG(LogTemp, Log, TEXT("ScoreWidget registered as observer."));
		}
	}
}
