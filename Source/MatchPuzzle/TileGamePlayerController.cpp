#include "TileGamePlayerController.h"
#include "Tile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "TileCommandInvoker.h"
#include "SwapTileCommand.h"
#include "TileGrid.h"

ATileGamePlayerController::ATileGamePlayerController()
{
    // 약한 참조로 타일을 관리 (처음에는 아무 것도 선택되지 않은 상태)
    FirstSelectedTile = nullptr;
    SecondSelectedTile = nullptr;
}

void ATileGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 마우스 커서 보이기 설정
    bShowMouseCursor = true;

    // Enhanced Input 설정
    if (APlayerController* PlayerController = Cast<APlayerController>(this))
    {
        // Enhanced Input 하위 시스템을 추가하여 InputMappingContext를 활성화
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(InputMapping, 0);  // 우선순위 0으로 설정
        }
    }
}

void ATileGamePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 마우스 클릭 입력 바인딩
        EnhancedInputComp->BindAction(SelectTileAction, ETriggerEvent::Started, this, &ATileGamePlayerController::SelectTile);
    }
}

void ATileGamePlayerController::SelectTile(const FInputActionValue& Value)
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        ATile* ClickedTile = Cast<ATile>(Hit.GetActor());
        if (ClickedTile)
        {
            // 첫 번째 타일 선택
            if (!FirstSelectedTile.IsValid())
            {
                FirstSelectedTile = ClickedTile;
                FirstSelectedTile->SetSelected(true);  // 첫 번째 타일 강조
                UE_LOG(LogTemp, Warning, TEXT("First tile selected: %s"), *FirstSelectedTile->GetName());
            }
            // 두 번째 타일 선택 (인접 타일만 가능)
            else if (!SecondSelectedTile.IsValid() && ClickedTile != FirstSelectedTile)
            {
                if (FirstSelectedTile->IsAdjacentTo(ClickedTile))  // 인접 타일인지 확인
                {
                    SecondSelectedTile = ClickedTile;
                    SecondSelectedTile->SetSelected(true);  // 두 번째 타일 강조
                    UE_LOG(LogTemp, Warning, TEXT("Second tile selected: %s"), *SecondSelectedTile->GetName());

                    // 두 타일 선택 완료 후 처리
                    ProcessSelectedTiles();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Tiles are not adjacent."));
                }
            }
        }
    }
}

void ATileGamePlayerController::ProcessSelectedTiles()
{
    if (FirstSelectedTile.IsValid() && SecondSelectedTile.IsValid())
    {
        // 두 개의 선택된 타일을 처리하는 로직

        // 교환 명령 생성
        USwapTileCommand* SwapCommand = NewObject<USwapTileCommand>(); // 클래스 이름 수정
        SwapCommand->Initialize(FirstSelectedTile.Get(), SecondSelectedTile.Get());

        // 커맨드 실행
        ATileCommandInvoker* CommandInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>();
        CommandInvoker->ExecuteCommand(SwapCommand);

        // 매칭 확인 로직 (교환 후 매칭이 있는지 확인)
        // MatchCheck(FirstSelectedTile, SecondSelectedTile);
        // 매칭된 타일 검사
       // TArray<ATile*> MatchingTiles = TileGrid->CheckForMatches();
       //
       // // 매칭된 타일이 있다면 삭제
       // if (MatchingTiles.Num() > 0)
       // {
       //     TileGrid->RemoveMatchingTiles(MatchingTiles);
       // }
        

        // 선택된 타일들 해제
        FirstSelectedTile->SetSelected(false);
        SecondSelectedTile->SetSelected(false);

        // 선택 초기화
        FirstSelectedTile = nullptr;
        SecondSelectedTile = nullptr;
    }
}
