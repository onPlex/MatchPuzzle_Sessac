// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"
#include "GI_Puzzle.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
	UE_LOG(LogTemp, Log, TEXT("OnNotify called with score: %d"), UpdateScore);

	UpdateSocreUI(UpdateScore);
}

void UGameWidgetObserver::UpdateSocreUI(int32 NewScore)
{
	// ScoreText가 바인딩되어 있는지 확인
	if (ScoreText)
	{
		// TextBlock의 내용을 업데이트
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ScoreText is not bound to the widget."));
	}
}
