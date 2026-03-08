# MultiMagicDungeon — Claude Project Context

## Project Overview
- **Engine:** Unreal Engine 5.7
- **Language:** C++ (primary), Blueprints (prototyping/data)
- **Genre:** Multiplayer magic dungeon crawler
- **Network model:** Dedicated server (listen-server fallback for dev)
- **Target players:** 2–4 co-op

## Key Conventions
- All networked gameplay logic lives in C++; Blueprints only for VFX/UI/data
- Use `UPROPERTY(Replicated)` and `GetLifetimeReplicatedProps` for all replicated variables
- Authority-only logic gated with `HasAuthority()`
- RPCs: Server RPCs for client→server, NetMulticast for server→all, Client RPCs for server→specific client
- `OnRep_` functions for replicated variable callbacks
- GameMode runs on server only; GameState replicated to all clients
- PlayerState carries per-player data (health, mana, spells)
- Prefer composition over inheritance; keep classes small and focused

## Module Dependencies to Add
- `OnlineSubsystem`, `OnlineSubsystemUtils` — matchmaking/sessions
- `NetCore` — networking primitives
- `GameplayAbilities`, `GameplayTags`, `GameplayTasks` — GAS for spells

## Workspace
Planning docs live in `docs/`. See `docs/PLAN.md` for the master roadmap.
Memory files live in `.claude/memory/`.

## Do Not
- Put gameplay authority logic in Blueprints
- Skip `HasAuthority()` guards on state-modifying functions
- Use `AActor::Tick` for network-sensitive state — use replication instead
- Commit `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`
