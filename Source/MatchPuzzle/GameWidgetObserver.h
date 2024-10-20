// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Observer.h"
#include "GameWidgetObserver.generated.h"
UCLASS()
class MATCHPUZZLE_API UGameWidgetObserver : public UUserWidget, public IObserver
{
	GENERATED_BODY()

private:
	// 현재 점수를 저장
	int32 CurrentScore;

public:
	// 주체로부터 상태 변화를 수신할 때 호출되는 함수
	virtual void OnNotify_Implementation(int32 UpdatedScore) override;

	// 점수를 UI에 반영하는 함수 (블루프린트에서 구현 가능)
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateScoreUI();
};
