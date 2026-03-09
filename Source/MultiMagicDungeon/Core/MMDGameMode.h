#pragma once

#include "GameFramework/GameModeBase.h"
#include "MMDGameMode.generated.h"

/**
 * Server-only. Manages game flow, player spawning, and win/lose conditions.
 * Runs exclusively on the server — never access from client code.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMMDGameMode();
};
