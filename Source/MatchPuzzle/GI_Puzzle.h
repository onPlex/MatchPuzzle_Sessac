// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Puzzle.generated.h"

class UGameStateSubject;
UCLASS()
class MATCHPUZZLE_API UGI_Puzzle : public UGameInstance
{
	GENERATED_BODY()
	UGI_Puzzle();

private:	
	//플레이 (플레이어) 점수	
	int32 PlayerScore;

	//게임 진행, 남은 매칭 횟수
	int32 RemaingMoves;

	// Observer 주체를 참조하는 변수 추가
	UGameStateSubject* GameStateSubjectInstance;
public:
	//점수 증가 메서드
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void AddScore(int32 point);

	//남은 이동 횟수 감소 메서드
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void DecreaseMoves();

	//게임 상태 초기화 (레벨 시작 시, 호출 - Retry)
	UFUNCTION(BlueprintCallable, Category = "Game Function")
	void ResetGameState();

	// 현재 점수를 가져오는 함수
	int32 GetCurrentScore() const;

	// 현재 점수를 가져오는 함수
	int32 GetRemainingMoves() const;

	// Observer 주체를 설정하는 함수
	void SetGameStateSubject(UGameStateSubject* Subject);
};
