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
	//Valid Check
	if (!FirstTile || !SecondTile)
	{
	    UE_LOG(LogTemp, Error, TEXT("Invalide Tiles for Swap"));
		return;
	}

	//타일 그리드 가져오기  
	ATileGrid* TileGrid = FirstTile->TileGrid;

	//그리드가 Valid 
	if (!TileGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalide TileGrid"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Execute"));
	// 타일 Swap
	TileGrid->SwapTiles(FirstTile, SecondTile);

	//타일 Swap 후, 매칭확인
	TArray<ATile*> MatchingTiles = TileGrid->CheckForMatches();
	if (MatchingTiles.Num() > 0) //매칭이 있을경우 
	{
	    //매칭된 타일 삭제
		TileGrid->RemoveMatchingTiles(MatchingTiles);
		UE_LOG(LogTemp, Warning, TEXT("Swap"));
	}
	else
	{
	    //매칭이 없을 경우, 타일을 제자리로 -> Undo()
		UE_LOG(LogTemp, Warning, TEXT("UNDO"));
		Undo();
	}
}

void USwapTileCommand::Undo()
{
	//타일 위치를 원위치로 되돌림 
	FirstTile->SetActorLocation(FirstTileOriginalLocation);
	SecondTile->SetActorLocation(SecondTileOriginalLocation);

	//그리드 내의 타일 좌표도 원래 좌표로 ~
	FVector2D TempPostion = FirstTile->TilePosition;
	FirstTile->TilePosition = SecondTile->TilePosition;
	SecondTile->TilePosition = TempPostion;

	//타일 그리드 가져오기  
	ATileGrid* TileGrid = FirstTile->TileGrid;
	if (TileGrid)
	{
		int32 x1, y1, x2, y2;
		if (TileGrid->GetTileGridPosition(FirstTile, x1, y1) && TileGrid->GetTileGridPosition(SecondTile, x2, y2))
		{
			TileGrid->SetTileAt(x1,y1, FirstTile);
			TileGrid->SetTileAt(x2, y2, SecondTile);
		}
	}

}