// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SwapTileCommand.h"
#include "Tile.h"
#include "TileCommandInvoker.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

ATileGamePlayerController::ATileGamePlayerController()
{
	// 선택된 타일 == nullptr , 약한 참조
	FirstSelecetTile == nullptr;
	SecondSelecetTile = nullptr;
}

void ATileGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//마우스 커서 보이기
	bShowMouseCursor = true;

	//EnhancedInput 실행
	if (APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		UEnhancedInputLocalPlayerSubsystem* _Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (_Subsystem)
		{
			_Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	// GetActorOfClass를 사용하여 TileGrid 찾기
	AActor* _FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), 
			ATileGrid::StaticClass());
	TileGrid = Cast<ATileGrid>(_FoundActor);

	
	if (!TileGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("TileGrid not found in the world."));
	}
}

void ATileGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(SelectInputAction, ETriggerEvent::Started, this,
		                              &ATileGamePlayerController::SelectTile);
	}
}

void ATileGamePlayerController::SelectTile(const FInputActionValue& Value)
{
	//마우스 클릭 위치를 가져오기
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		ATile* ClickedTile = Cast<ATile>(Hit.GetActor());
		if (ClickedTile)
		{
			if (!FirstSelecetTile.IsValid())
			{
				FirstSelecetTile = ClickedTile;
				FirstSelecetTile->SetSelected(true);
				UE_LOG(LogTemp, Display, TEXT("First Select Tile: %s"), *FirstSelecetTile->GetName());
			}
			else if (!SecondSelecetTile.IsValid() && ClickedTile != FirstSelecetTile)
			{
				if (FirstSelecetTile->IsAdjacentTo(ClickedTile))
				{
					//두번째 타일 선택
					SecondSelecetTile = ClickedTile;
					SecondSelecetTile->SetSelected(true);
					UE_LOG(LogTemp, Display, TEXT("Second Select Tile: %s"), *SecondSelecetTile->GetName());


					//두번째 타일 선택 완료 후 처리 - 두개다 골라졌을때
					ProcessSelectTiles();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Tiles are not adjacent"));
				}
			}
		}
	}
}

void ATileGamePlayerController::ProcessSelectTiles()
{
	if (!FirstSelecetTile.IsValid() || !SecondSelecetTile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid tiles selected."));
		return;
	}

	if(!FirstSelecetTile->IsAdjacentTo(SecondSelecetTile.Get()))
	{
		UE_LOG(LogTemp, Error, TEXT("Tiles are not adjacent"));
		return;;
	}

	// 교환 명령 생성
	USwapTileCommand* SwapCommand = NewObject<USwapTileCommand>();
	SwapCommand->Initialize(FirstSelecetTile.Get(), SecondSelecetTile.Get());

	ATileCommandInvoker* CommandInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>();
	CommandInvoker->ExecuteCommand(SwapCommand);

	// 타일 선택 해제
	if(FirstSelecetTile.IsValid())FirstSelecetTile->SetSelected(false);
	if(SecondSelecetTile.IsValid())SecondSelecetTile->SetSelected(false);

	// 선택 초기화
	if(FirstSelecetTile.IsValid())FirstSelecetTile = nullptr;
	if(SecondSelecetTile.IsValid())SecondSelecetTile = nullptr;
}
