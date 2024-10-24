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

	//현재 점수 변수
	//GameInstance GI_Puzzle 에서 대체
	//int32 CurrentScore;
protected:
	// UI에서 바인딩된 TextBlock을 가져오기 위한 변수 선언
	// BindWidget 어노테이션을 사용하여 Blueprint의 위젯을 바인딩
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

public:
	//주체로부터 상태변화를 수신할 때 호출하는 함수
	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	UFUNCTION()
	void UpdateSocreUI(int32 NewScore);
};
