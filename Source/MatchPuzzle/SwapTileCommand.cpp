#include "SwapTileCommand.h"
#include "Tile.h"

// 커맨드 초기화
void USwapTileCommand::Initialize(ATile* InFirstTile, ATile* InSecondTile)
{
	FirstTile = InFirstTile;
	SecondTile = InSecondTile;

	// 원래 위치를 저장
	FirstTileOriginalLocation = FirstTile->GetActorLocation();
	SecondTileOriginalLocation = SecondTile->GetActorLocation();
}

void USwapTileCommand::Execute()
{
	// 두 타일의 위치를 서로 교환
	FirstTile->SetActorLocation(SecondTileOriginalLocation);
	SecondTile->SetActorLocation(FirstTileOriginalLocation);
	
	// TODO:: 매칭 확인 등의 로직
	

	//그리드 좌표도 교체 
	FVector2D TempPosition = FirstTile->TilePosition;
	FirstTile->UpdateTilePosition(SecondTile->TilePosition);
	SecondTile->UpdateTilePosition(TempPosition);
}

void USwapTileCommand::Undo()
{
	// 타일의 위치를 원래대로 되돌림
	FirstTile->SetActorLocation(FirstTileOriginalLocation);
	SecondTile->SetActorLocation(SecondTileOriginalLocation);

	//그리드 좌표 원래대로 돌림
	FVector2D TempPosition = FirstTile->TilePosition;
	FirstTile->UpdateTilePosition(SecondTile->TilePosition);
	SecondTile->UpdateTilePosition(TempPosition);
}