// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Observer.h"
#include "GameStateSubject.generated.h"

/**
 *  역활 : 주체 클래스 -> 상태를 모니터링하고, 변경 시 옵저버들에게 알려주는 역활 
 */
UCLASS()
class MATCHPUZZLE_API UGameStateSubject : public UObject
{
	GENERATED_BODY()
	
private:	
	// 옵저버 리시트: 등록된 옵저버들 -> 관리
	TArray<TScriptInterface<IObserver>> Observers;		
	//주체의 상태정보 -> 플레이어 점수
	int32 PlayerScore;
	
public:
      // 생성자
     UGameStateSubject();
     
     //옵저버 등록 
     void RegisterObserver(TScriptInterface<IObserver> Observer);
	
	 //옵저버 등록 해제	 
	 void UnregisterObserver(TScriptInterface<IObserver> Observer);
	 
	 //점수 변화시, 모든 옵저버들에게 알림
	 void NotifyObservers();
	 
	 //점수를 변경하는 함수,
	 void IncreasePlayerScore(int32 Increament);
	 
	 //점수 반환
	 int32 GetPlayerScore() const {return PlayerScore;}
};
