#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "TileGamePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATile;
class ATileGrid;

UCLASS()
class MATCHPUZZLE_API ATileGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATileGamePlayerController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 마우스 커서 보이기
	virtual void SetupInputComponent() override;

	// 첫 번째와 두 번째로 선택된 타일을 약한 참조로 저장 (GC 대응)
	TWeakObjectPtr<ATile> FirstSelectedTile; // +참고 _)UPROPERTY()를 사용하면 Unreal Engine의 가비지 컬렉션에 의해 관리되므로, 안전하게 참조를 유지
	TWeakObjectPtr<ATile> SecondSelectedTile; // +참고 _) UPROPERTY()를 사용하면 Unreal Engine의 가비지 컬렉션에 의해 관리되므로, 안전하게 참조를 유지
	//TWeakObjectPtr를 사용하면 가비지 컬렉션으로 삭제된 객체가 있는지 체크할 수 있습니다. 객체가 유효하지 않을 경우 자동으로 nullptr로 설정
	 

	// TileGrid에 대한 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	ATileGrid* TileGrid;

	// Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SelectTileAction;

	// 타일을 선택하는 함수
	void SelectTile(const FInputActionValue& Value);

	// 두 개의 타일을 선택하고 처리하는 함수
	void ProcessSelectedTiles();
};
