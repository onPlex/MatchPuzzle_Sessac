// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Match3GameMode.generated.h"

class UGameStateSubject;
UCLASS()
class MATCHPUZZLE_API AMatch3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	

	//Observer 주체 생성 
	//UGameStateSubject* ObseverGameState;
};
