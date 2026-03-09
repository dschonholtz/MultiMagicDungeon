#pragma once

#include "GameFramework/Character.h"
#include "MMDPlayerCharacter.generated.h"

class AMMDPlayerState;

/**
 * The player's physical presence in the world.
 * Movement is replicated by the CharacterMovementComponent automatically.
 *
 * Does NOT store persistent data (health, mana, spells) — that lives on AMMDPlayerState.
 * Phase 2+: GAS AbilitySystemComponent accessed via PlayerState.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMDPlayerCharacter();

	/** Convenience accessor — avoids repeated casts at call sites. */
	AMMDPlayerState* GetMMDPlayerState() const;

protected:
	virtual void BeginPlay() override;
};
