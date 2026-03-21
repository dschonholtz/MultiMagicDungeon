#include "Core/MMDPlayerState.h"
#include "MMDLog.h"
#include "Net/UnrealNetwork.h"

AMMDPlayerState::AMMDPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	SetNetUpdateFrequency(10.f);
}

void AMMDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMMDPlayerState, Health);
	DOREPLIFETIME(AMMDPlayerState, Mana);
}

void AMMDPlayerState::SetHealth(float NewHealth)
{
	if (!HasAuthority()) return;
	Health = NewHealth;
}

void AMMDPlayerState::SetMana(float NewMana)
{
	if (!HasAuthority()) return;
	Mana = NewMana;
}

void AMMDPlayerState::OnRep_Health()
{
}

void AMMDPlayerState::OnRep_Mana()
{
}
