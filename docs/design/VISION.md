# Game Vision

_Core design decisions. Update when the vision changes; note the date and reason._

---

## Core Loop (2026-03-08)

Players inhabit a persistent open world. Each player can **build a dungeon** that exists as a physical place in that world. Other players can **find and raid** those dungeons. The flow is:

```
Overworld roam → find dungeon entrance → enter instance → raid → return to overworld
```

This is the north star. Every architecture decision should be checked against it.

---

## Player Count (2026-03-08)

- **Session target:** 2–8 players per dungeon run (start with 2–4, raise cap later)
- **Overworld:** Eventually all players on same persistent world (MMO-style)
- **Decision:** Build session-based first to prove fun, then layer in persistence. Do not design the session layer in a way that blocks the overworld transition.

---

## Architecture Tier Decision (2026-03-08)

We build in phases:

| Phase | Model | Infrastructure |
|---|---|---|
| 0–2 | Session-based co-op | Single server process |
| 3–4 | Session + cloud saves | Server + simple backend API + DB |
| 5+ | Persistent overworld | Always-on world server + instance pool + backend |

**Why not start with MMO architecture?** Persistent world requires backend services, database, instance orchestration — before the core game is fun. We validate the dungeon-raiding loop first.

**Why won't this require a rewrite?** UE5 Seamless Travel is additive. GameMode/GameState/PlayerState survive the session→persistent transition with refactoring, not replacement. The dungeon code is the same whether it's a session or an instance.

---

## Dungeon Ownership (2026-03-08)

- Dungeons are **player-authored** (editor TBD — tile-based? freeform? procgen seeded?)
- Dungeon layout persists server-side (database-backed in Phase 4+)
- Owner can update/rebuild their dungeon between raids
- Dungeon has a **difficulty rating** and **loot table** set by the owner
- Open question: PvP flagging inside dungeons? Leave for later.

---

## Tech Stack (2026-03-08)

- Engine: UE 5.7
- Language: C++ authority, Blueprints for VFX/UI/data only
- Spells: Gameplay Ability System (GAS)
- Sessions (Phase 0–2): OnlineSubsystemNull (LAN/dev)
- Sessions (Phase 3+): Steam or EOS (decision deferred)
- Backend (Phase 4+): TBD — likely a lightweight REST API + PostgreSQL
- Dedicated server: Required for Phase 5; listen server acceptable for Phase 0–2
