#pragma once

#include "GameFramework/GameStateBase.h"
#include "MMDGameState.generated.h"

/**
 * Replicated to all clients. Holds world-level state visible to everyone:
 * game phase, match timer, dungeon seed. Extended per phase.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMMDGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
