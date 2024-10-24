#include "SwapTileCommand.h"
#include "Tile.h"
#include "TileGrid.h"

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
	if (!FirstTile || !SecondTile)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Tiles for swapping."));
		return;
	}

	// FirstTile에서 TileGrid 가져오기
	ATileGrid* TileGrid = FirstTile->TileGrid;
    
	// 디버그: TileGrid가 유효한지 확인
	if (!TileGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve TileGrid from the tile."));
		return;
	}

	// 그리드에서 타일을 교환
	TileGrid->SwapTiles(FirstTile, SecondTile);

	// 매칭 확인 및 후속 작업 수행
	TArray<ATile*> MatchingTiles = TileGrid->CheckForMatches();
	if (MatchingTiles.Num() > 0)
	{
		TileGrid->RemoveMatchingTiles(MatchingTiles);
	}
	else
	{
		// 매칭이 없을 경우 다시 타일을 원래 위치로 되돌림 (Undo 로직 호출 가능)
		Undo();
	}
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
