#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MMDAttributeSet.generated.h"

// Generates getter, setter, and init functions for each attribute.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Core combat attributes. Owned by PlayerState's ASC.
 * Health and Mana are the only attributes for Phase 2.
 */
UCLASS()
class MULTIMAGICDUNGEON_API UMMDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMMDAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "MMD|Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMMDAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "MMD|Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMMDAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "MMD|Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMMDAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "MMD|Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMMDAttributeSet, MaxMana)

private:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
};
