// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Observer.h"
#include "GameWidgetObserver.generated.h"

/**
 * 
 */
UCLASS()
class MATCHPUZZLE_API UGameWidgetObserver : public UUserWidget, public IObserver
{
	GENERATED_BODY()

	
private:
	//���� ���� ����
	int32 CurrentScore;

public:

    //��ü�κ��� ���º�ȭ�� ������ �� ȣ���ϴ� �Լ�
	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	// BlueprintImplementableEvent = �������Ʈ���� �Ǳ��� ���� 
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateSocreUI();
	
};
