#pragma once

#include "SpellSystem/MMDSpellTypes.h"

/**
 * Stateless spell chain interpreter. Iterates a chain, builds context,
 * and returns a result describing what game effect to create.
 * Server-only — never run on clients.
 */
namespace MMDSpellExecution
{
	/**
	 * Execute a spell chain starting from an initial context.
	 * Returns FMMDSpellResult describing the effect to create.
	 */
	FMMDSpellResult Execute(const FMMDSpellChain& Chain, const FMMDSpellContext& InitialContext);
}
