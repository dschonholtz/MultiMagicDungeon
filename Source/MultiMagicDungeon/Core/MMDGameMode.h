#pragma once

#include "GameFramework/GameMode.h"
#include "MMDGameMode.generated.h"

/**
 * Server-only. Manages game flow, player spawning, and win/lose conditions.
 * Runs exclusively on the server — never access from client code.
 *
 * Inherits from AGameMode (not AGameModeBase) so that PostLogin calls
 * RestartPlayer, which spawns the DefaultPawnClass for each joining player.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMMDGameMode();

protected:
	virtual void BeginPlay() override;
};
