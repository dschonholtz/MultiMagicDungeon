#include "SpellSystem/MMDSpellChainComponent.h"
#include "MMDLog.h"

UMMDSpellChainComponent::UMMDSpellChainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

bool UMMDSpellChainComponent::AddPrimitive(const FMMDSpellPrimitive& Primitive)
{
	if (CurrentChain.Primitives.Num() >= MaxChainLength)
	{
		UE_LOG(LogMMD, Warning, TEXT("SpellChain: max chain length (%d) reached"), MaxChainLength);
		return false;
	}

	CurrentChain.Primitives.Add(Primitive);
	OnChainChanged.Broadcast();
	return true;
}

void UMMDSpellChainComponent::CancelChain()
{
	if (CurrentChain.Primitives.Num() > 0)
	{
		CurrentChain.Reset();
		OnChainChanged.Broadcast();
	}
}

FMMDSpellChain UMMDSpellChainComponent::FinalizeChain(const FMMDSpellPrimitive& TerminalPrimitive)
{
	CurrentChain.Primitives.Add(TerminalPrimitive);
	PendingCast = CurrentChain;
	CurrentChain.Reset();
	OnChainChanged.Broadcast();
	return PendingCast;
}
