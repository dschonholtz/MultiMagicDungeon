#include "Core/MMDGameState.h"
#include "Net/UnrealNetwork.h"

AMMDGameState::AMMDGameState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMMDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Replicated properties added here as game state grows (TASK-005+).
}
