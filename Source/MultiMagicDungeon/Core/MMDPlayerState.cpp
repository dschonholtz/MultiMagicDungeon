#include "Core/MMDPlayerState.h"
#include "AbilitySystem/MMDAbilitySystemComponent.h"
#include "AbilitySystem/MMDAttributeSet.h"
#include "AbilitySystem/MMDSpellCastAbility.h"
#include "MMDLog.h"

AMMDPlayerState::AMMDPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	SetNetUpdateFrequency(10.f);

	AbilitySystemComponent = CreateDefaultSubobject<UMMDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMMDAttributeSet>(TEXT("AttributeSet"));
}

void AMMDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Server grants abilities
	if (HasAuthority() && AbilitySystemComponent)
	{
		FGameplayAbilitySpec SpellCastSpec(UMMDSpellCastAbility::StaticClass(), 1);
		AbilitySystemComponent->GiveAbility(SpellCastSpec);
		UE_LOG(LogMMD, Log, TEXT("PlayerState: granted SpellCastAbility"));
	}
}

UAbilitySystemComponent* AMMDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AMMDPlayerState::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.f;
}

float AMMDPlayerState::GetMana() const
{
	return AttributeSet ? AttributeSet->GetMana() : 0.f;
}
