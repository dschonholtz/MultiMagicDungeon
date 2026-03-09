#pragma once

#include "GameFramework/PlayerState.h"
#include "MMDPlayerState.generated.h"

/**
 * Per-player replicated state. Persists across Character respawns.
 * All authoritative player data lives here, not on the Character.
 *
 * Phase 0: Health, Mana stubs with replication wired up.
 * Phase 2+: GAS AbilitySystemComponent moves here for persistence.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMMDPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "MMD|Stats")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "MMD|Stats")
	float GetMana() const { return Mana; }

	/** Server-only. Sets health and replicates to clients. */
	void SetHealth(float NewHealth);

	/** Server-only. Sets mana and replicates to clients. */
	void SetMana(float NewMana);

private:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	float Mana = 100.f;

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Mana();
};
