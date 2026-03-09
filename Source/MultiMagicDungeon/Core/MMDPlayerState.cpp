#include "Core/MMDPlayerState.h"
#include "MMDLog.h"
#include "Net/UnrealNetwork.h"

AMMDPlayerState::AMMDPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	NetUpdateFrequency = 10.f;
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
	UE_LOG(LogMMD, Verbose, TEXT("%s Health -> %.1f"), *GetPlayerName(), Health);
}

void AMMDPlayerState::OnRep_Mana()
{
	UE_LOG(LogMMD, Verbose, TEXT("%s Mana -> %.1f"), *GetPlayerName(), Mana);
}
