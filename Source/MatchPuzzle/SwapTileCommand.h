#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "Tile.h"
#include "SwapTileCommand.generated.h"

UCLASS()
class MATCHPUZZLE_API USwapTileCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:
	ATile* FirstTile;
	ATile* SecondTile;

	// 교환 전 타일의 위치
	FVector FirstTileOriginalLocation;
	FVector SecondTileOriginalLocation;

public:
	// 생성자
	void Initialize(ATile* InFirstTile, ATile* InSecondTile);

	// ICommand 인터페이스 구현
	virtual void Execute() override;
	virtual void Undo() override;
};
