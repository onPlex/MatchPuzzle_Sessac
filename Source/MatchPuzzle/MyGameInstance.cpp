// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	//초기 점수 + 이동 점수
    PlayerScore = 0;
	RemaingMoves = 30;
}

void UMyGameInstance::AddScore(int32 point)
{
	PlayerScore += point;
}

void UMyGameInstance::DecreaseMoves()
{
	//남은 움직이기 횟수가 있으면 
	if(RemaingMoves > 0)
	{
		RemaingMoves--;
	}
}

void UMyGameInstance::ResetGameState()
{
	PlayerScore = 0;
	RemaingMoves = 30;
}
