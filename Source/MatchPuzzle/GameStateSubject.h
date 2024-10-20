#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Observer.h"
#include "GameStateSubject.generated.h"

/**
 * 주체 클래스 - 상태를 관리하고 변경 시 옵저버들에게 알림을 보냄
 * 이 클래스는 UObject를 상속받아 언리얼 엔진의 메모리 관리와 통합됨
 */
UCLASS()
class MATCHPUZZLE_API UGameStateSubject : public UObject
{
	GENERATED_BODY()

private:
	// 옵저버 리스트: 등록된 옵저버들이 상태 변화를 수신함
	TArray<TScriptInterface<IObserver>> Observers;

	// 주체의 상태 정보 (예: 플레이어 점수)
	int32 PlayerScore;

public:
	// 생성자: 초기 점수 설정
	UGameStateSubject();

	// 옵저버 등록: 주체가 상태 변화를 알릴 옵저버를 등록함
	void RegisterObserver(TScriptInterface<IObserver> Observer);

	// 옵저버 등록 해제: 상태 변화 알림을 중지할 옵저버를 제거함
	void UnregisterObserver(TScriptInterface<IObserver> Observer);

	// 상태 변화 발생 시 모든 옵저버에게 알림
	void NotifyObservers();

	// 상태(점수)를 변경하는 함수. 상태가 변경되면 NotifyObservers()가 호출됨
	void IncreaseScore(int32 Amount);

	// 현재 점수 반환
	int32 GetScore() const { return PlayerScore; }
};
