#include "Character/MMDPlayerCharacter.h"
#include "Core/MMDPlayerState.h"
#include "MMDLog.h"

AMMDPlayerCharacter::AMMDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMMDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

AMMDPlayerState* AMMDPlayerCharacter::GetMMDPlayerState() const
{
	return GetPlayerState<AMMDPlayerState>();
}
