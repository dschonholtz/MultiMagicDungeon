#include "Character/MMDPlayerController.h"
#include "MMDLog.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMMDPlayerController::AMMDPlayerController()
{
	// PlayerController MUST tick — PlayerTick() runs ProcessPlayerInput()
	// Setting bCanEverTick=false silently kills ALL input processing

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder(
		TEXT("/Game/Input/IMC_Default"));
	DefaultMappingContext = IMCFinder.Object;
}

void AMMDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// Game-only input mode — without this, keyboard events don't reach the pawn
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		UE_LOG(LogMMD, Warning, TEXT("PC::BeginPlay: SetInputMode(GameOnly) done"));
	}
}

void AMMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Sub->AddMappingContext(DefaultMappingContext, 0);
		UE_LOG(LogMMD, Warning, TEXT("PC::SetupInput: IMC=%s HasIt=%s"),
			*GetNameSafe(DefaultMappingContext),
			Sub->HasMappingContext(DefaultMappingContext) ? TEXT("Y") : TEXT("N"));
	}
	else
	{
		UE_LOG(LogMMD, Error, TEXT("PC::SetupInput: NO Enhanced Input subsystem"));
	}
}
