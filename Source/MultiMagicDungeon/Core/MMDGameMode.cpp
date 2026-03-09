#include "Core/MMDGameMode.h"
#include "Core/MMDGameState.h"
#include "Core/MMDPlayerState.h"
#include "Character/MMDPlayerCharacter.h"
#include "Character/MMDPlayerController.h"
#include "Online/MMDSessionSubsystem.h"
#include "UI/MMDHUD.h"
#include "MMDLog.h"

AMMDGameMode::AMMDGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = true;

	GameStateClass        = AMMDGameState::StaticClass();
	PlayerStateClass      = AMMDPlayerState::StaticClass();
	DefaultPawnClass      = AMMDPlayerCharacter::StaticClass();
	PlayerControllerClass = AMMDPlayerController::StaticClass();
	HUDClass              = AMMDHUD::StaticClass();
}

void AMMDGameMode::BeginPlay()
{
	Super::BeginPlay();

	// GameMode only runs on server — safe to create session here
	if (UMMDSessionSubsystem* Sessions = GetGameInstance()->GetSubsystem<UMMDSessionSubsystem>())
	{
		Sessions->CreateSession();
	}
}
