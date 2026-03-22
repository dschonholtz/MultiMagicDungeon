#pragma once

#include "AbilitySystemComponent.h"
#include "MMDAbilitySystemComponent.generated.h"

/**
 * Thin ASC subclass. Lives on PlayerState for persistence across respawns.
 * Character implements IAbilitySystemInterface and delegates here.
 */
UCLASS()
class MULTIMAGICDUNGEON_API UMMDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMMDAbilitySystemComponent();
};
