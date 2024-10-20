// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdatedScore)
{
	// 주체로부터 새로운 점수를 받아서 CurrentScore를 업데이트
	CurrentScore = UpdatedScore;

	// UI 업데이트 (블루프린트에서 구현 가능)
	UpdateScoreUI();
}