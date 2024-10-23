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
	// 유효성 확인
	if (!FirstTile || !SecondTile)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Tiles for swapping."));
		return;
	}

	// 타일 그리드 가져오기 (FirstTile의 소속 그리드)
	ATileGrid* TileGrid = FirstTile->TileGrid;
    
	// 그리드가 유효한지 확인
	if (!TileGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve TileGrid from the tile."));
		return;
	}

	// 그리드 내에서 타일을 교환
	TileGrid->SwapTiles(FirstTile, SecondTile);

	// 타일 교환 후 매칭 확인
	TArray<ATile*> MatchingTiles = TileGrid->CheckForMatches();
	if (MatchingTiles.Num() > 0)  // 매칭이 있을 경우
	{
		TileGrid->RemoveMatchingTiles(MatchingTiles);  // 매칭된 타일 삭제
	}
	else  // 매칭이 없을 경우
	{
		// 타일을 원래 위치로 되돌림
		Undo();
	}
}

void USwapTileCommand::Undo()
{
	// 타일의 위치를 원래대로 되돌림
	FirstTile->SetActorLocation(FirstTileOriginalLocation);
	SecondTile->SetActorLocation(SecondTileOriginalLocation);

	// 그리드 내의 타일 좌표도 원래대로 돌림
	FVector2D TempPosition = FirstTile->TilePosition;
	FirstTile->TilePosition = SecondTile->TilePosition;
	SecondTile->TilePosition = TempPosition;

	// 그리드 배열 업데이트
	ATileGrid* TileGrid = FirstTile->TileGrid;
	if (TileGrid)
	{
		int32 X1, Y1, X2, Y2;
		if (TileGrid->GetTileGridPosition(FirstTile, X1, Y1) && TileGrid->GetTileGridPosition(SecondTile, X2, Y2))
		{
			TileGrid->SetTileAt(X1, Y1, FirstTile);
			TileGrid->SetTileAt(X2, Y2, SecondTile);
		}
	}
}