#include "GameStateSubject.h"
#include "GI_Puzzle.h"
#include "Kismet/GameplayStatics.h"

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
void UGameStateSubject::NotifyObservers(UObject* WorldContextObject)
{
    // 옵저버 리스트에서 유효하지 않은 옵저버 제거
    Observers.RemoveAll([](const TScriptInterface<IObserver>& Observer)
    {
        if (!IsValid(Observer.GetObject()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid observer detected, removing from list."));
            return true; // 유효하지 않으면 제거
        }
        return false; // 유효하면 유지
    });

    // WorldContextObject가 nullptr이 아닌지 확인
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Error, TEXT("WorldContextObject is null."));
        return;
    }

    // GameInstance 가져오기
    UGI_Puzzle* GameInstance = Cast<UGI_Puzzle>(UGameplayStatics::GetGameInstance(WorldContextObject));

    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance from WorldContextObject"));
        return;
    }

    int32 CurrentScore = GameInstance->GetCurrentScore();

    // 옵저버들에게 점수 업데이트 알림
    for (const TScriptInterface<IObserver>& Observer : Observers)
    {
        if (IsValid(Observer.GetObject())
            && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
        {
            // 옵저버에게 알림
            IObserver::Execute_OnNotify(Observer.GetObject(), CurrentScore);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid observer or interface not implemented."));
        }
    }
}

void UGameStateSubject::NotifyObserversMoves(UObject* WorldContextObject)
{
    Observers.RemoveAll([](const TScriptInterface<IObserver>& Observer)
      {
           return !IsValid(Observer.GetObject());
     });

    // WorldContextObject가 nullptr이 아닌지 확인
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Error, TEXT("WorldContextObject is null."));
        return;
    }

    // GameInstance 가져오기
    UGI_Puzzle* GameInstance = Cast<UGI_Puzzle>(UGameplayStatics::GetGameInstance(WorldContextObject));


    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance from WorldContextObject"));
        return;
    }

    int32 RemainingMoves = GameInstance->GetRemainingMoves();

    // 옵저버들에게 점수 업데이트 알림
    for (const TScriptInterface<IObserver>& Observer : Observers)
    {
        if (IsValid(Observer.GetObject())
            && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
        {
            // 옵저버에게 알림
            IObserver::Execute_OnNotifyRemainingMoves(Observer.GetObject(), RemainingMoves);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid observer or interface not implemented."));
        }
    }

}
