#pragma once

#include "Abilities/GameplayAbility.h"
#include "SpellSystem/MMDSpellTypes.h"
#include "MMDSpellCastAbility.generated.h"

/**
 * GameplayAbility that interprets and executes a spell chain.
 * One instance handles all spell types — the chain determines behavior.
 * Reads the pending chain from the Avatar's SpellChainComponent.
 * ServerOnly execution — client sends chain via Server RPC on Character.
 */
UCLASS()
class MULTIMAGICDUNGEON_API UMMDSpellCastAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMMDSpellCastAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	float ComputeManaCost(const FMMDSpellChain& Chain) const;
	void ApplyManaCost(UAbilitySystemComponent* ASC, float Cost);
	void SpawnProjectile(const FMMDSpellResult& Result, AActor* AvatarActor);
	void ApplyAoE(const FMMDSpellResult& Result, AActor* AvatarActor);

	float ManaCostPerPrimitive = 10.f;
};
