# TASK-008: GAS Foundation (ASC + AttributeSet)

**Status:** IN_PROGRESS
**Phase:** 2 — Composable Spell System
**Created:** 2026-03-21
**Completed:** —

---

## Goal

Wire up the Gameplay Ability System infrastructure: AbilitySystemComponent on PlayerState,
AttributeSet with Health/MaxHealth/Mana/MaxMana, IAbilitySystemInterface on Character.
This replaces the raw Health/Mana floats with GAS-managed attributes and prepares
the foundation for the spell system.

Also fixes pre-existing HUD rendering issue.

## Acceptance Criteria
- [ ] UMMDAbilitySystemComponent created on PlayerState via CreateDefaultSubobject
- [ ] UMMDAttributeSet with Health, MaxHealth, Mana, MaxMana using ATTRIBUTE_ACCESSORS
- [ ] PlayerState implements IAbilitySystemInterface
- [ ] Character implements IAbilitySystemInterface (delegates to PlayerState)
- [ ] InitAbilityActorInfo called in PossessedBy (server) and OnRep_PlayerState (client)
- [ ] Raw Health/Mana removed from PlayerState (replaced by AttributeSet)
- [ ] GetHealth()/GetMana() convenience wrappers read from AttributeSet
- [ ] Debug HUD displays health/mana from AttributeSet
- [ ] Project compiles without errors or new warnings
- [ ] PIE 2-player smoke test: health/mana visible for both players

## Files to Create
- `Source/MultiMagicDungeon/AbilitySystem/MMDAbilitySystemComponent.h` — thin ASC subclass
- `Source/MultiMagicDungeon/AbilitySystem/MMDAbilitySystemComponent.cpp`
- `Source/MultiMagicDungeon/AbilitySystem/MMDAttributeSet.h` — GAS attributes
- `Source/MultiMagicDungeon/AbilitySystem/MMDAttributeSet.cpp`

## Files to Change
- `Source/MultiMagicDungeon/Core/MMDPlayerState.h/.cpp` — add ASC + AttributeSet, remove raw floats
- `Source/MultiMagicDungeon/Character/MMDPlayerCharacter.h/.cpp` — add IAbilitySystemInterface, InitAbilityActorInfo
- `Source/MultiMagicDungeon/UI/MMDHUD.cpp` — update health/mana reading if API changes

## Test Plan
1. Build MultiMagicDungeonEditor
2. Open UE editor → PIE with 2 players (Listen Server)
3. Confirm debug HUD shows player names, health (100), and mana (100)
4. Both players see each other's stats

## Notes
- Build.cs already has GameplayAbilities, GameplayTags, GameplayTasks modules
- HUD health/mana not currently rendering (pre-existing bug) — fix here
- Design doc: docs/design/MAGIC.md

## Review Checklist (fill in before marking DONE)
- [ ] `/review` run — verdict: ___
- [ ] `/simplify` run — changes applied: ___
- [ ] Compile: `Result: Succeeded`
- [ ] Tests: N passed, 0 failed
- [ ] `docs/PROGRESS.md` updated
- [ ] `docs/design/ROADMAP.md` task status updated
