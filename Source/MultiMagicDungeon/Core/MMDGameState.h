#pragma once

#include "GameFramework/GameState.h"
#include "MMDGameState.generated.h"

/**
 * Replicated to all clients. Holds world-level state visible to everyone:
 * game phase, match timer, dungeon seed. Extended per phase.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMMDGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
