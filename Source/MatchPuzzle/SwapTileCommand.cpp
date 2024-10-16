// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTileCommand.h"
#include "Tile.h"

SwapTileCommand::SwapTileCommand()
{

}

SwapTileCommand::~SwapTileCommand()
{
}

SwapTileCommand::SwapTileCommand(ATile* InFirstTile, ATile* InSecondTile)
{
    FirstTile = InFirstTile;
    SecondTile = InSecondTile;

    FirstTileOrigineType = InFirstTile->TileType;
    SecondTileOrigineType = InSecondTile->TileType;
}

void SwapTileCommand::Execute()
{
    //SWAP , 위치 Vector (Location ) ,행렬 (Row ,Collums -> Int , Int)
    FName Temp = FirstTile->TileType;
    FirstTile->TileType = SecondTile->TileType;
    SecondTile->TileType = Temp;
}

void SwapTileCommand::Undo()
{
    FirstTile->TileType = FirstTileOrigineType;
    SecondTile->TileType = SecondTileOrigineType;
}
