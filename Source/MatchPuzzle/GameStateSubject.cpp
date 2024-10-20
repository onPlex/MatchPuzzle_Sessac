// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"


UGameStateSubject ::UGameStateSubject()
{
    PlayerScore = 0;
}

//옵저버 등록 
void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
    Observers.Add(Observer);
}

//옵저버 등록 해제	
void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
    Observers.Remove(Observer);
}

//점수 변화시, 모든 옵저버들에게 알림
void UGameStateSubject::NotifyObservers()
{
    // 등록된 Observers 들 반복문으로 확인 후,
    for (TScriptInterface<IObserver> Observer : Observers)
    {
        // Observer 객체체가 유효하고 && IObserver 인터페이스를 구현하고 있는지 확인
        if (Observer.GetObject() 
        && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
        {
            // 옵저버의 OnNotify 함수 호출 
            // 실질적인 클래스 인스턴스 -> 함수를 호출
            // 인터페이스 -> 호출 , 의존성이 확낮아짐 
            IObserver::Execute_OnNotify(Observer.GetObject(),PlayerScore);
        }
    }
}

//점수를 변경하는 함수,
void UGameStateSubject::IncreasePlayerScore(int32 Increament)
{
    PlayerScore += Increament;
    NotifyObservers();
}
