#pragma once

#include "Components/ActorComponent.h"
#include "SpellSystem/MMDSpellTypes.h"
#include "MMDSpellChainComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellChainChanged);

/**
 * Local-only component that tracks the spell chain being built in real time.
 * Lives on Character. No replication — chain only crosses the network
 * when the cast button is pressed (via GAS ability activation).
 */
UCLASS(ClassGroup = (MMD), meta = (BlueprintSpawnableComponent))
class MULTIMAGICDUNGEON_API UMMDSpellChainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMMDSpellChainComponent();

	/** Add a primitive to the current chain. Returns false if chain is full. */
	bool AddPrimitive(const FMMDSpellPrimitive& Primitive);

	/** Clear the chain without casting. */
	void CancelChain();

	/** Get the current chain (read-only). */
	const FMMDSpellChain& GetCurrentChain() const { return CurrentChain; }

	/** True if at least one primitive has been added. */
	bool IsChainActive() const { return CurrentChain.Primitives.Num() > 0; }

	/** Finalize the chain with a terminal instruction, store in PendingCast, then clear. */
	FMMDSpellChain FinalizeChain(const FMMDSpellPrimitive& TerminalPrimitive);

	/** The last finalized chain, ready for the ability to consume. */
	const FMMDSpellChain& GetPendingCast() const { return PendingCast; }
	void SetPendingCast(const FMMDSpellChain& Chain) { PendingCast = Chain; }
	void ClearPendingCast() { PendingCast.Reset(); }

	UPROPERTY(BlueprintAssignable)
	FOnSpellChainChanged OnChainChanged;

private:
	static constexpr int32 MaxChainLength = 8;

	FMMDSpellChain CurrentChain;
	FMMDSpellChain PendingCast;
};
