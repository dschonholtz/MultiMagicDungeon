# TASK-002: Base C++ Framework Classes

**Status:** IN_REVIEW
**Phase:** 0 — Foundation
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Create the five skeleton C++ classes that form the server/client authority framework.
No gameplay logic — just correct base classes, replication boilerplate, and log category.
These classes must exist before any gameplay feature can be built.

## Acceptance Criteria
- [x] `AMMDGameMode` — inherits `AGameModeBase`, server-only, tick disabled
- [x] `AMMDGameState` — inherits `AGameStateBase`, `GetLifetimeReplicatedProps` implemented
- [x] `AMMDPlayerState` — inherits `APlayerState`, `Health`/`Mana` replicated with `OnRep_`
- [x] `AMMDPlayerCharacter` — inherits `ACharacter`, movement replicated via CMC
- [x] `AMMDPlayerController` — inherits `APlayerController`, tick disabled
- [x] Shared log category `LogMMD` declared and defined
- [x] Project compiles without errors
- [ ] PIE 2-player: both characters spawn, move independently — manual post-merge

## Files Created
- `Source/MultiMagicDungeon/Core/MMDGameMode.h/.cpp`
- `Source/MultiMagicDungeon/Core/MMDGameState.h/.cpp`
- `Source/MultiMagicDungeon/Core/MMDPlayerState.h/.cpp`
- `Source/MultiMagicDungeon/Character/MMDPlayerCharacter.h/.cpp`
- `Source/MultiMagicDungeon/Character/MMDPlayerController.h/.cpp`
- `Source/MultiMagicDungeon/MMDLog.h` — shared log category

## Design Notes
- `AMMDPlayerState` holds Health and Mana as the authoritative store.
  Character does NOT hold these — Character can be destroyed/respawned, PlayerState persists.
- No GAS wiring yet (TASK-008). ASC placeholder noted in comments only.
- `AMMDGameMode` sets `bUseSeamlessTravel = true` now — cheap to set, avoids a required
  change later that could affect save data if forgotten.
- `DefaultPawnClass` and `PlayerStateClass` set on GameMode constructor.

## Review Checklist
- [x] `/review` — APPROVE
- [x] `/simplify` — no dead code, no premature abstractions
- [x] Compile: `Result: Succeeded`
- [ ] PIE 2-player smoke test — manual, post-merge
- [x] `docs/PROGRESS.md` updated
