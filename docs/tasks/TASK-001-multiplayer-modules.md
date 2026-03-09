# TASK-001: Add Multiplayer Module Dependencies

**Status:** PLANNED
**Phase:** 0 — Foundation
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Add the Unreal module and plugin dependencies required for multiplayer, networking, and the Gameplay Ability System. No gameplay code changes — compile verification only.

## Acceptance Criteria
- [ ] `Build.cs` includes `OnlineSubsystem`, `OnlineSubsystemUtils`, `NetCore`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`
- [ ] `.uproject` enables `OnlineSubsystem`, `OnlineSubsystemNull` (LAN), `GameplayAbilities` plugins
- [ ] Project compiles without errors or warnings introduced by this change
- [ ] PIE launches successfully with 2 players

## Files to Change
- `Source/MultiMagicDungeon/MultiMagicDungeon.Build.cs`
- `MultiMagicDungeon.uproject`

## Test Plan
1. Open project in UE 5.7
2. Compile (`Ctrl+Shift+B` or Live Coding)
3. PIE with `Number of Players: 2`, `Net Mode: Play As Listen Server`
4. Confirm no errors in Output Log

## Notes
- `OnlineSubsystemNull` is the LAN/dev backend — swap for Steam/EOS later without touching game code
- Do NOT add `OnlineSubsystemSteam` yet — adds Steam SDK dependency we don't need in Phase 0
- `GameplayAbilities` module is large; expect slightly longer compile on first build
