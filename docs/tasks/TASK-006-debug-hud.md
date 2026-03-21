# TASK-006: Debug HUD

**Status:** IN_REVIEW
**Phase:** 1 — Network Framework
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Visible proof that replication works: every client sees all players' names,
health, and mana drawn on screen. No UMG assets required — pure Canvas/DrawText.
Replace with UMG widgets in Phase 1.

## Acceptance Criteria
- [x] `AMMDHUD` draws player list (name, HP, MP) from `GameState->PlayerArray`
- [x] Local player's row shown in green, others in white
- [x] Crosshair drawn at screen centre
- [x] `AMMDGameMode` sets `HUDClass = AMMDHUD`
- [x] Compile clean
- [ ] PIE: both windows show both players' stats updating — manual post-merge

## Additional Work (same branch)

**Enhanced Input migration** — replaced broken Tick-based `IsInputKeyDown` polling with
proper `UEnhancedInputComponent` bindings. Required to unblock PIE movement testing.
- `UInputAction` (Move/Look/Jump) and `UInputMappingContext` created programmatically in constructor (no editor assets)
- Mapping context registered in `BeginPlay` via `UEnhancedInputLocalPlayerSubsystem`
- `PrimaryActorTick.bCanEverTick = false`

## Review Checklist
- [x] `/review` — APPROVE: DrawHUD fires client-side only, PlayerArray read-only
- [x] `/simplify` — two private helpers, each one job
- [x] Compile: `Result: Succeeded`
- [x] `/review` (Enhanced Input) — APPROVE: no authority issues, UPROPERTY guards all input objects
- [x] `/simplify` (Enhanced Input) — no removals needed, constructor init is appropriately sized
- [x] Compile (Enhanced Input): `Result: Succeeded`
