// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tile.h"

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
	if(APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		UEnhancedInputLocalPlayerSubsystem* _Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if(_Subsystem)
		{
			_Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void ATileGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(SelectInputAction,ETriggerEvent::Started,this, &ATileGamePlayerController::SelectTile);
	}
}

void ATileGamePlayerController::SelectTile(const FInputActionValue& Value)
{
	//마우스 클릭 위치를 가져오기
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if(Hit.bBlockingHit)
	{
		ATile* ClickedTile = Cast<ATile>(Hit.GetActor());
		if(ClickedTile)
		{
			if(!FirstSelecetTile.IsValid())
			{
				FirstSelecetTile = ClickedTile;
				UE_LOG(LogTemp, Display, TEXT("First Select Tile: %s"),*FirstSelecetTile->GetName());
			}
			else if(!SecondSelecetTile.IsValid() && ClickedTile != FirstSelecetTile)
			{
				//두번째 타일 선택
				SecondSelecetTile = ClickedTile;
				UE_LOG(LogTemp, Display, TEXT("Second Select Tile: %s"),*SecondSelecetTile->GetName());
				//두번째 타일 선택 완료 후 처리
				ProcessSelectTiles();
			}
		}
	
	}
}

void ATileGamePlayerController::ProcessSelectTiles()
{
	//두 개의 타일이 선택되었을 때
	if(FirstSelecetTile.IsValid() && SecondSelecetTile.IsValid())
	{
		//타일 처리 로직 ( 자리교환, 매칭확인 )

		//선택 초기화
		FirstSelecetTile = nullptr;
		SecondSelecetTile = nullptr;
	}
}
