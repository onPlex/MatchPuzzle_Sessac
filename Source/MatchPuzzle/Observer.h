#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Observer.generated.h"

// 이 클래스를 UCLASS가 아닌 UINTERFACE로 선언하여 인터페이스로 정의
UINTERFACE(MinimalAPI)
class UObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * IObserver 인터페이스 정의
 * 모든 옵저버들은 이 인터페이스를 구현하여 주체로부터 상태 변화를 수신함
 */
class MATCHPUZZLE_API IObserver
{
	GENERATED_BODY()

public:
	// 주체로부터 상태 변화를 수신하는 함수
	// BlueprintNativeEvent로 정의되어, C++ 또는 블루프린트에서 구현 가능
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Observer")
	void OnNotify(int32 UpdatedScore);
};
