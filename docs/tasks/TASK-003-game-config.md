# TASK-003: Game Config

**Status:** IN_REVIEW
**Phase:** 0 — Foundation
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Wire DefaultGame.ini to our GameMode and configure OnlineSubsystem=Null so the engine
uses AMMDGameMode for all sessions and the LAN backend is active.

## Acceptance Criteria
- [x] `DefaultGame.ini` sets `GlobalDefaultGameMode` to `AMMDGameMode`
- [x] `DefaultEngine.ini` configures `OnlineSubsystem=Null` and `OnlineSubsystemNull` enabled
- [x] Compile clean

## Review Checklist
- [x] `/review` — APPROVE (config only)
- [x] `/simplify` — N/A
- [x] Compile: `Result: Succeeded`
