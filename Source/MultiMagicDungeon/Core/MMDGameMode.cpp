#include "Core/MMDGameMode.h"
#include "Core/MMDGameState.h"
#include "Core/MMDPlayerState.h"
#include "Character/MMDPlayerCharacter.h"
#include "Character/MMDPlayerController.h"

AMMDGameMode::AMMDGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = true;

	GameStateClass       = AMMDGameState::StaticClass();
	PlayerStateClass     = AMMDPlayerState::StaticClass();
	DefaultPawnClass     = AMMDPlayerCharacter::StaticClass();
	PlayerControllerClass = AMMDPlayerController::StaticClass();
}
