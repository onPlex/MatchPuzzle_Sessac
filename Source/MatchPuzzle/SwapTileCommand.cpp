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
    // 타일 위치 교환 (월드 좌표)
    FirstTile->SetActorLocation(SecondTileOriginalLocation);
    SecondTile->SetActorLocation(FirstTileOriginalLocation);

    // 그리드 좌표도 함께 교환
    FVector2D TempPosition = FirstTile->TilePosition;
    FirstTile->UpdateTilePosition(SecondTile->TilePosition);
    SecondTile->UpdateTilePosition(TempPosition);
    // TODO:: 매칭 확인 등의 로직
}

void USwapTileCommand::Undo()
{
    // 타일 위치를 원래대로 되돌림 (월드 좌표)
    FirstTile->SetActorLocation(FirstTileOriginalLocation);
    SecondTile->SetActorLocation(SecondTileOriginalLocation);

    // 그리드 좌표도 원래대로 되돌림
    FVector2D TempPosition = FirstTile->TilePosition;
    FirstTile->UpdateTilePosition(SecondTile->TilePosition);
    SecondTile->UpdateTilePosition(TempPosition);
}
