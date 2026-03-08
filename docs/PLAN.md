# MultiMagicDungeon — Master Multiplayer Plan

_Last updated: 2026-03-08_

## Vision
2–4 player co-op dungeon crawler with magic combat. Players explore procedurally generated (or hand-crafted) dungeons, cast spells, fight enemies, and progress together.

---

## Architecture Overview

```
┌─────────────────────────────────────────┐
│              SERVER (Authority)          │
│  GameMode · AIController · SpawnManager  │
│  CombatResolver · DungeonGenerator      │
└────────────┬────────────────────────────┘
             │  Replication
   ┌──────────┴──────────┐
   │     GAME STATE      │  (replicated to all)
   │  Score · Phase · Map│
   └──────────┬──────────┘
              │
  ┌───────────┴───────────┐
  │    PLAYER STATE (x4)  │  (per-player, replicated)
  │  Health · Mana · Spells│
  └───────────┬───────────┘
              │
  ┌───────────┴───────────┐
  │   PLAYER CHARACTER    │  (possessed by each client)
  │  Movement · Input · GAS│
  └───────────────────────┘
```

---

## Milestones

### Phase 0 — Foundation (Current)
- [x] Project created (UE 5.7)
- [x] Git repo initialized
- [x] .claude workspace setup
- [ ] Add multiplayer module dependencies to Build.cs
- [ ] Enable OnlineSubsystem plugin in .uproject
- [ ] Create base GameMode, GameState, PlayerState classes in C++
- [ ] Set up listen-server map for local dev testing

### Phase 1 — Network Framework
- [ ] Implement `AMMDGameMode` (server-only, manages game flow)
- [ ] Implement `AMMDGameState` (replicated: phase, timer, dungeon seed)
- [ ] Implement `AMMDPlayerState` (replicated: health, mana, team slot)
- [ ] Implement `AMMDPlayerCharacter` (movement replication, authority guards)
- [ ] Session creation / joining (LAN first, then OnlineSubsystem)
- [ ] Basic HUD showing connected players

### Phase 2 — Gameplay Ability System (GAS) for Spells
- [ ] Add GAS modules: `GameplayAbilities`, `GameplayTags`, `GameplayTasks`
- [ ] `UMMDAbilitySystemComponent` on PlayerCharacter and PlayerState
- [ ] Define base `UMMDGameplayAbility` (spell base class)
- [ ] Implement 3 starter spells: Fireball, FrostBolt, Telekinesis
- [ ] Attribute Set: Health, MaxHealth, Mana, MaxMana, SpellPower
- [ ] Gameplay Effects for damage, healing, status effects

### Phase 3 — Dungeon & Enemies
- [ ] Dungeon room/tile system (server-authoritative generation)
- [ ] Replicate dungeon layout to clients via GameState or custom replication
- [ ] Base enemy AI (BTTree, EQS) with server-authoritative logic
- [ ] Enemy health/death replicated; visual death handled client-side
- [ ] Loot system (items replicated, pickup authority on server)

### Phase 4 — Polish & Sessions
- [ ] Lobby system (player ready-up, character selection)
- [ ] Steam / EOS session integration
- [ ] Latency compensation for projectile spells
- [ ] Reconnect handling (PlayerState persistence)
- [ ] Win/fail conditions, level progression

---

## File Structure (Target)

```
Source/MultiMagicDungeon/
├── Core/
│   ├── MMDGameMode.h/.cpp
│   ├── MMDGameState.h/.cpp
│   └── MMDPlayerState.h/.cpp
├── Character/
│   ├── MMDPlayerCharacter.h/.cpp
│   └── MMDPlayerController.h/.cpp
├── AbilitySystem/
│   ├── MMDAbilitySystemComponent.h/.cpp
│   ├── MMDAttributeSet.h/.cpp
│   ├── Abilities/
│   └── Effects/
├── Dungeon/
│   ├── MMDDungeonGenerator.h/.cpp
│   └── MMDRoom.h/.cpp
├── Enemy/
│   ├── MMDEnemyBase.h/.cpp
│   └── MMDEnemyAIController.h/.cpp
└── UI/
    └── MMDHUD.h/.cpp
```

---

## Naming Convention
- Classes: `AMMD` (Actors), `UMMD` (UObjects/Components), `FMMD` (structs)
- Prefix all project classes with `MMD`
- Blueprints: `BP_MMD_*`
- Data Assets: `DA_MMD_*`
- Gameplay Tags: `MMD.Spell.*`, `MMD.Status.*`, `MMD.Character.*`

---

## Good Practices Checklist
- [ ] All server-only logic guarded with `HasAuthority()`
- [ ] All replicated properties have `GetLifetimeReplicatedProps` entries
- [ ] No gameplay logic in Blueprints (only data/VFX/UI)
- [ ] Ability costs/cooldowns enforced server-side via GAS
- [ ] Enemy AI runs only on server; results replicated
- [ ] Session/matchmaking abstracted behind an interface for easy backend swap
