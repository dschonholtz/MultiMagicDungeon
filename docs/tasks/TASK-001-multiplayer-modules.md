# TASK-001: Add Multiplayer Module Dependencies

**Status:** IN_REVIEW
**Phase:** 0 — Foundation
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Add the Unreal module and plugin dependencies required for multiplayer, networking, and the Gameplay Ability System. No gameplay code changes — compile verification only.

## Acceptance Criteria
- [x] `Build.cs` includes `OnlineSubsystem`, `OnlineSubsystemUtils`, `NetCore`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`
- [x] `.uproject` enables `OnlineSubsystem`, `OnlineSubsystemNull` (LAN), `GameplayAbilities` plugins
- [x] Project compiles without errors or warnings introduced by this change (`Result: Succeeded`)
- [ ] PIE launches successfully with 2 players — **verify manually after merge**

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
- `GameplayTags` and `GameplayTasks` are transitive deps of `GameplayAbilities` but declared explicitly — guards against Epic changing transitive resolution

## Review Checklist
- [x] `/review` — APPROVE (no C++ Actor code, config-only change)
- [x] `/simplify` — removed empty PrivateDependencyModuleNames and stale comments
- [x] Compile: `Result: Succeeded` (9.45s)
- [ ] PIE 2-player smoke test — manual, post-merge
- [x] `docs/PROGRESS.md` updated
