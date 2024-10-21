#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Async\ParallelFor.h"
// Sets default values
ATile::ATile()
{
	// StaticMeshComponent 생성
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	RootComponent = TileMesh; // 타일의 루트 컴포넌트로 설정

	// 타일 타입 초기화
	TileType = FName("Cube");  // 기본 타입을 Cube로 설정 (나중에 랜덤하게 변경)

	bIsSelected = false;  // 선택 여부 초기화
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	// 타일 외형 업데이트 (TileType에 따라 메시 변경)
	UpdateTileAppearance();
}

bool ATile::IsMatching(ATile* OtherTile) const
{
	// 타일 타입이 일치하는지 확인
	return TileType == OtherTile->TileType;
}

void ATile::UpdateTileAppearance()
{
	if (TileMeshes.Contains(TileType))
	{
		// TileType에 맞는 Static Mesh를 설정
		TileMesh->SetStaticMesh(TileMeshes[TileType]);
	}
	else
	{
		// TileType에 맞는 메시가 없으면 기본 메시를 설정
		UE_LOG(LogTemp, Warning, TEXT("TileType %s has no mesh set"), *TileType.ToString());
	}
}

void ATile::ProceesDataInParallel()
{
	TArray<int32> DataArray;
	DataArray.Init(0, 100);

	ParallelFor(DataArray.Num(), [&](int32 Index)
	{
		DataArray[Index] = Index * 2;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Tile %d - %d"), Index, DataArray[Index]));
		}
	});

	UE_LOG(LogTemp, Warning, TEXT("ParallelFor Finish"));
}

void ATile::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;
	UpdateAppearance();
}

void ATile::UpdateAppearance()
{
	// 선택되었을 때의 시각적 피드백 (예: 색상 변경)
	if (bIsSelected)
	{
		// 타일이 선택되었을 때의 효과
		// 예: StaticMeshComponent의 색상 변경
		if (UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>())
		{
			MeshComponent->SetRenderCustomDepth(true);  // 선택된 타일을 강조
			MeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 10.0f);  // 강조된 색상 변경
		}
	}
	else
	{
		if (UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>())
		{
			MeshComponent->SetRenderCustomDepth(false);  // 기본 상태로 복귀
			MeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 0.0f);  // 기본 색상 복귀
		}
	}
}
