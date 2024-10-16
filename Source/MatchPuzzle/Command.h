// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Command.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommand : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MATCHPUZZLE_API ICommand
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    //명령 실행 
    virtual void Execute() = 0;    
    //명령 취소 --> Puzzle 게임 한정 ~= 블록 되돌아가기
    virtual void Undo() = 0;
    
    //virtual ~ICommand()= default;
};
