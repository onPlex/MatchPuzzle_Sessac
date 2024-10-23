// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include"Components/StaticMeshComponent.h"


// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Static 메쉬 컴포넌트 생성
	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
	RootComponent = TileMeshComponent;

	TileType = FName("Default");
	bIsSelected = false;
	
	//그리드상의 초기화 위치
	TilePosition = FVector2D::ZeroVector;
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	UpdateTileAppearance();
}



bool ATile::IsMatching(ATile* otherTile) const
{
	return TileType == otherTile->TileType;
}

bool ATile::IsMatcing(ATile* OtherTile) const
{
	// 같은 타일인지 여부 -> OtherTile 이랑 같은 타입인가 ?
	return TileType == OtherTile->TileType;
}

void ATile::UpdateTileAppearance()
{
	//TileMeshes Map에 TileType에 해당하는 키값이 존재하는가 ?
	if (TileMeshes.Contains(TileType))
	{
		TileMeshComponent->SetStaticMesh(TileMeshes[TileType]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attempting to update tile appearance"));
	}
}


void ATile::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;
	UpdateAppearance();
}
void ATile::UpdateAppearance()
{
	//선택되었을 때, 시각적 피드백 - > Material Emissive
	if(bIsSelected)
	{
		if(UStaticMeshComponent* StaticMeshComponent = TileMeshComponent)
		{
			//선택된 타일을 강조
			TileMeshComponent->SetRenderCustomDepth(true);
			//EmissiveStrength 제어를 통한 하이라이트 구현
			TileMeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 10.0f);
		}		
	}
	else
	{
		if(UStaticMeshComponent* StaticMeshComponent = TileMeshComponent)
		{
			//선택된 타일을 강조
			TileMeshComponent->SetRenderCustomDepth(false);
			//EmissiveStrength 제어를 통한 하이라이트 구현
			TileMeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 0.0f);
		}	
	}
}

bool ATile::IsAdjacentTo(ATile* OtherTile) const
{
	if (!OtherTile) return false;

	// 두 타일의 그리드 좌표 차이를 계산하여 인접 여부 확인
	int32 DeltaX = FMath::Abs(TilePosition.X - OtherTile->TilePosition.X);
	int32 DeltaY = FMath::Abs(TilePosition.Y - OtherTile->TilePosition.Y);

	// 두 타일이 가로 또는 세로로 1칸 차이일 경우 인접한 것으로 판단
	return (DeltaX + DeltaY) == 1;
}

void ATile::UpdateTilePosition(const FVector2D& NewPosition)
{
	TilePosition = NewPosition;
}
