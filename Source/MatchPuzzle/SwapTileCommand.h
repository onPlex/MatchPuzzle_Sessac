// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"

class ATile;
class MATCHPUZZLE_API SwapTileCommand : public ICommand
{
public:
	SwapTileCommand();
	~SwapTileCommand();
	

	SwapTileCommand(ATile* InFirstTile, ATile* InSecondTile);
	//FirstTile(InFirstTile), SecondTile(InSecondTile)
private:
	ATile* FirstTile;
	ATile* SecondTile;
	FName FirstTileOrigineType;
	FName SecondTileOrigineType;
	
public:

    //ICommand 의 인터페이스 함수들 Imprement ~ 
    virtual void Execute() override;
    virtual void Undo() override;


};
