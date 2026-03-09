# TASK-005: LAN Session Manager

**Status:** IN_REVIEW
**Phase:** 1 — Network Framework
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Server creates an LAN session on BeginPlay so clients can discover and connect.
For now clients connect via `open <IP>` in console. FindSessions/JoinSession UI
comes in a later task.

## How to Test Multiplayer Locally
1. PIE: Editor → Play dropdown → set `Number of Players: 2`, `Net Mode: Play As Listen Server`
2. Packaged: run server with `?listen`, client connects with console cmd `open 127.0.0.1`

## Acceptance Criteria
- [x] `UMMDSessionSubsystem` creates LAN session with 8 slots on server BeginPlay
- [x] Session delegates properly cleaned up after complete
- [x] `AMMDGameMode::BeginPlay` calls `CreateSession` (server-only path)
- [x] Compile clean

## Files Created/Changed
- `Source/MultiMagicDungeon/Online/MMDSessionSubsystem.h/.cpp` (new)
- `Source/MultiMagicDungeon/Core/MMDGameMode.h/.cpp` (BeginPlay added)

## Review Checklist
- [x] `/review` — APPROVE: delegate handles cleared in callbacks, HasAuthority implicit via GameMode
- [x] `/simplify` — single responsibility: subsystem owns session only
- [x] Compile: `Result: Succeeded`
