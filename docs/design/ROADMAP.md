# Roadmap

_High-level phases. Individual tasks live in `docs/tasks/`. Update phases as scope changes; note the date._

---

## Phase 0 — Foundation
_Goal: Project compiles with multiplayer modules. PIE works with 2 players._

| Task | Status |
|---|---|
| [TASK-001: Add Multiplayer Module Dependencies](../tasks/TASK-001-multiplayer-modules.md) | PLANNED |
| TASK-002: Base C++ classes (GameMode, GameState, PlayerState, Character) | PLANNED |
| TASK-003: Dev listen-server map + PIE smoke test | PLANNED |

---

## Phase 1 — Network Framework
_Goal: Players can connect, see each other move, and have replicated health/mana._

- TASK-004: Replicated movement (verify CMC works over network)
- TASK-005: PlayerState replication (health, mana visible to all)
- TASK-006: Session creation and joining (LAN, OnlineSubsystemNull)
- TASK-007: Basic HUD showing connected players and stats

---

## Phase 2 — Gameplay Ability System (Spells)
_Goal: 3 spells working with proper server authority, costs, cooldowns._

- TASK-008: GAS setup (AbilitySystemComponent, AttributeSet)
- TASK-009: Fireball ability
- TASK-010: FrostBolt ability
- TASK-011: Telekinesis ability
- TASK-012: Damage numbers and spell VFX (client-side, Multicast)

---

## Phase 3 — Dungeon + Enemies
_Goal: A playable dungeon loop with AI enemies and loot._

- TASK-013: Dungeon room system (server-authoritative layout)
- TASK-014: Dungeon layout replication to clients
- TASK-015: Base enemy AI (BehaviorTree, server-only)
- TASK-016: Enemy health/death replication
- TASK-017: Basic loot (server-authoritative pickup)

---

## Phase 4 — Persistence + Backend (Supabase)
_Goal: Player progress and dungeon layouts survive sessions. Supabase as backend._

- TASK-018: Supabase project setup + schema (players, dungeons, inventory tables)
- TASK-019: UE HTTP client wrapper for Supabase REST API (`UMMDSupabaseClient`)
- TASK-020: Player data persistence (save/load on session join/leave)
- TASK-021: Dungeon layout persistence (save player's dungeon, load on entry)
- TASK-022: Dungeon registry query (list available dungeons to raid)
- TASK-023: Dungeon creation tools (in-engine tile editor, TBD design)

---

## Phase 5 — Dedicated Server + Hetzner
_Goal: Packaged server binary running on Linux VPS. Real clients connecting._

- TASK-024: Linux cross-compile toolchain setup
- TASK-025: Server target file (`MultiMagicDungeonServer.Target.cs`)
- TASK-026: Package + deploy to Hetzner VPS (Ubuntu 22.04)
- TASK-027: Startup scripts + basic server monitoring

---

## Phase 6 — Persistent Overworld
_Goal: Always-on world, dungeons as physical places, seamless travel._

- TASK-028: Overworld map with dungeon entrance actors
- TASK-029: Seamless travel overworld ↔ dungeon instance (`bUseSeamlessTravel`)
- TASK-030: Instance pool (spin up/down dungeon server processes on demand)
- TASK-031: Session/EOS or Steam integration for player discovery

---

## Naming Convention

| Pattern | Use |
|---|---|
| `AMMD` | Actor subclasses |
| `UMMD` | UObject/Component subclasses |
| `FMMD` | Structs |
| `BP_MMD_*` | Blueprints |
| `DA_MMD_*` | Data Assets |
| `MMD.Spell.*` | Gameplay Tags (spells) |
| `MMD.Status.*` | Gameplay Tags (status effects) |
| `MMD.Character.*` | Gameplay Tags (character attributes) |
