// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Puzzle.h"
#include "GameStateSubject.h"

UGI_Puzzle::UGI_Puzzle()
{
	//초기 점수 + 이동 점수
	PlayerScore = 0;
	RemaingMoves = 30;
	GameStateSubjectInstance = nullptr;
}

void UGI_Puzzle::AddScore(int32 point)
{
	PlayerScore += point;

	UE_LOG(LogTemp, Log, TEXT("Score added. New score: %d"), PlayerScore);

	if (GameStateSubjectInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Notifying observers."));
		GameStateSubjectInstance->NotifyObservers(this);  // 'this'를 WorldContextObject로 전달
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameStateSubjectInstance is null. Cannot notify observers."));
	}
}

void UGI_Puzzle::DecreaseMoves()
{
	//남은 움직이기 횟수가 있으면 
	if(RemaingMoves > 0)
	{
		RemaingMoves--;

		if (GameStateSubjectInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Notifying observers."));
			GameStateSubjectInstance->NotifyObserversMoves(this); // 'this'를 WorldContextObject로 전달
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameStateSubjectInstance is null. Cannot notify observers."));
		}
	}
}

void UGI_Puzzle::ResetGameState()
{
	PlayerScore = 0;
	RemaingMoves = 30;
}

int32 UGI_Puzzle::GetCurrentScore() const
{
	return PlayerScore;
}

int32 UGI_Puzzle::GetRemainingMoves() const
{
	return RemaingMoves;
}

void UGI_Puzzle::SetGameStateSubject(UGameStateSubject* Subject)
{
	GameStateSubjectInstance = Subject;
}
