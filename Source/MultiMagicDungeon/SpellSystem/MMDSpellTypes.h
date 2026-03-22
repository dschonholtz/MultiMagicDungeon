#pragma once

#include "GameplayTagContainer.h"
#include "MMDSpellTypes.generated.h"

/**
 * The type of a spell primitive instruction.
 * Source primitives acquire material, modifiers shape it,
 * terminal primitives create game effects.
 */
UENUM(BlueprintType)
enum class EMMDSpellPrimitiveType : uint8
{
	// Source
	Summon,		// Create raw element

	// Modifier
	Shape,		// Give material a form

	// Terminals
	Launch,		// Propel forward (projectile)
	Detonate,	// Release energy as AoE
};

/**
 * A single instruction in a spell chain.
 */
USTRUCT(BlueprintType)
struct MULTIMAGICDUNGEON_API FMMDSpellPrimitive
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMMDSpellPrimitiveType Type = EMMDSpellPrimitiveType::Summon;

	/** Parameter tag — e.g., MMD.Element.Fire for Summon, MMD.Shape.Sphere for Shape */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Parameter;

	FMMDSpellPrimitive() = default;

	FMMDSpellPrimitive(EMMDSpellPrimitiveType InType, FGameplayTag InParam = FGameplayTag())
		: Type(InType), Parameter(InParam)
	{
	}

	/** True if this is a terminal instruction (ends the chain and produces an effect). */
	bool IsTerminal() const
	{
		return Type == EMMDSpellPrimitiveType::Launch
			|| Type == EMMDSpellPrimitiveType::Detonate;
	}
};

/**
 * An ordered sequence of spell primitives.
 */
USTRUCT(BlueprintType)
struct MULTIMAGICDUNGEON_API FMMDSpellChain
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMMDSpellPrimitive> Primitives;

	/** True if the chain has a terminal instruction. */
	bool IsComplete() const
	{
		return Primitives.Num() > 0 && Primitives.Last().IsTerminal();
	}

	/** Basic grammar validation. */
	bool IsValid() const
	{
		if (Primitives.Num() == 0) return false;
		if (!IsComplete()) return false;

		// Must start with a source primitive
		if (Primitives[0].Type != EMMDSpellPrimitiveType::Summon)
		{
			return false;
		}

		// Terminal must be last
		for (int32 i = 0; i < Primitives.Num() - 1; ++i)
		{
			if (Primitives[i].IsTerminal()) return false;
		}

		return true;
	}

	void Reset()
	{
		Primitives.Empty();
	}
};

/**
 * The type of game effect produced by executing a spell chain.
 */
UENUM(BlueprintType)
enum class EMMDSpellResultType : uint8
{
	None,
	Projectile,		// Spawn a flying projectile
	AoE,			// Area of effect at a position
};

/**
 * Mutable context built up during chain execution.
 * Each primitive reads/writes this like CPU registers.
 */
USTRUCT(BlueprintType)
struct MULTIMAGICDUNGEON_API FMMDSpellContext
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Element;

	UPROPERTY()
	FGameplayTag Shape;

	UPROPERTY()
	FVector Origin = FVector::ZeroVector;

	UPROPERTY()
	FVector Direction = FVector::ForwardVector;

	UPROPERTY()
	float Power = 1.f;
};

/**
 * The result of executing a spell chain — describes what to spawn.
 */
USTRUCT(BlueprintType)
struct MULTIMAGICDUNGEON_API FMMDSpellResult
{
	GENERATED_BODY()

	UPROPERTY()
	EMMDSpellResultType Type = EMMDSpellResultType::None;

	UPROPERTY()
	FGameplayTag Element;

	UPROPERTY()
	FGameplayTag Shape;

	UPROPERTY()
	FVector Origin = FVector::ZeroVector;

	UPROPERTY()
	FVector Direction = FVector::ForwardVector;

	UPROPERTY()
	float Power = 1.f;

	UPROPERTY()
	float Radius = 300.f;
};
