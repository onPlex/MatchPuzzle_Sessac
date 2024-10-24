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
	// ScoreText�� ���ε��Ǿ� �ִ��� Ȯ��
	if (ScoreText)
	{
		// TextBlock�� ������ ������Ʈ
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ScoreText is not bound to the widget."));
	}
}
