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

	//���� ���� ����
	//GameInstance GI_Puzzle ���� ��ü
	//int32 CurrentScore;
protected:
	// UI���� ���ε��� TextBlock�� �������� ���� ���� ����
	// BindWidget ������̼��� ����Ͽ� Blueprint�� ������ ���ε�
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;

public:
	//��ü�κ��� ���º�ȭ�� ������ �� ȣ���ϴ� �Լ�
	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	UFUNCTION()
	void UpdateSocreUI(int32 NewScore);
};
