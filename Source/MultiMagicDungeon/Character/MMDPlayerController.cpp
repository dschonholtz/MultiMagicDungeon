#include "Character/MMDPlayerController.h"
#include "MMDLog.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMMDPlayerController::AMMDPlayerController()
{
	PrimaryActorTick.bCanEverTick = false;

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
	}
}

void AMMDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Sub->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		UE_LOG(LogMMD, Error, TEXT("PC::SetupInput: NO Enhanced Input subsystem"));
	}
}
