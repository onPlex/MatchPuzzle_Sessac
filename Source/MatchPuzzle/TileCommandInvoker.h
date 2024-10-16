// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Command.h"
#include "TileCommandInvoker.generated.h"

UCLASS()
class MATCHPUZZLE_API ATileCommandInvoker : public AActor
{
	GENERATED_BODY()

private:
	TArray<ICommand*> CommandHistory; 
public:	
	// Sets default values for this actor's properties
	ATileCommandInvoker();
	~ATileCommandInvoker();

public:
	 //커맨드 실행하고, 히스토리(TArray) 에 저장
	 void ExecuteCommand(ICommand* Command);

	//마지막 명령을 취소 (Undo)
	void UndoLastCommand();
	
};
