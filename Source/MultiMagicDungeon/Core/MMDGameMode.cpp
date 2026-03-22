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
	// Spawn the Blueprint subclass (mesh/anims set in editor, not hardcoded in C++)
	static ConstructorHelpers::FClassFinder<AMMDPlayerCharacter> BPFinder(
		TEXT("/Game/BP_MMDPlayerCharacter"));
	if (BPFinder.Succeeded())
	{
		DefaultPawnClass = BPFinder.Class;
	}
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
