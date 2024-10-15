// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MATCHPUZZLE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UMyGameInstance();

public:
	//게임에 전체적으로 필요한 변수들

	//플레이 (플레이어) 점수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 PlayerScore;

	//게임 진행, 남은 매칭 횟수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 RemaingMoves;

	//점수 증가 메서드
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void AddScore(int32 point);

	//남은 이동 횟수 감소 메서드
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void DecreaseMoves();


	//게임 상태 초기화 (레벨 시작 시, 호출 - Retry)
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void ResetGameState();
};
