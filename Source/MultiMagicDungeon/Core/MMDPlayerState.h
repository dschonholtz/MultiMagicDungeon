#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MMDPlayerState.generated.h"

class UMMDAbilitySystemComponent;
class UMMDAttributeSet;

/**
 * Per-player replicated state. Persists across Character respawns.
 * Owns the AbilitySystemComponent and AttributeSet for GAS.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMMDPlayerState();

	virtual void BeginPlay() override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UMMDAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Convenience wrappers — read from AttributeSet. */
	UFUNCTION(BlueprintPure, Category = "MMD|Stats")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "MMD|Stats")
	float GetMana() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "MMD|Abilities")
	TObjectPtr<UMMDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMMDAttributeSet> AttributeSet;
};
