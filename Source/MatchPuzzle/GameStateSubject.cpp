#include "GameStateSubject.h"

UGameStateSubject::UGameStateSubject()
{
	// 초기 점수 설정
	PlayerScore = 0;
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	// 옵저버를 리스트에 추가
	Observers.Add(Observer);
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	// 옵저버를 리스트에서 제거
	Observers.Remove(Observer);
}

void UGameStateSubject::NotifyObservers()
{
	// 등록된 모든 옵저버들에게 상태 변화를 알림
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		// Observer 객체가 유효하고 IObserver 인터페이스를 구현하고 있는지 확인
		if (Observer.GetObject() && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			// 옵저버의 OnNotify 함수 호출 (점수 변화를 전달)
			IObserver::Execute_OnNotify(Observer.GetObject(), PlayerScore);
		}
	}
}

void UGameStateSubject::IncreaseScore(int32 Amount)
{
	// 점수를 증가시키고 상태 변화를 옵저버들에게 알림
	PlayerScore += Amount;
	NotifyObservers();
}
