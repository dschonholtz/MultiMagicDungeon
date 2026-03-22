# MultiMagicDungeon — C++ & UE Coding Conventions

Deep-reference for coding standards. The agent onboarding entry point is the root `CLAUDE.md`.

---

## Replication Rules (Non-Negotiable)

```cpp
// Every replicated variable needs both of these:
UPROPERTY(ReplicatedUsing=OnRep_Health)
float Health;

virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
// In .cpp: DOREPLIFETIME(AMMDPlayerState, Health);

// Every state-mutating function must gate on authority:
void AMMDPlayerCharacter::TakeDamage(float Amount)
{
    if (!HasAuthority()) return;
    // ... server-only logic
}
```

**RPC signatures:**
```cpp
UFUNCTION(Server, Reliable, WithValidation)
void Server_CastSpell(FGameplayTag SpellTag, FHitResult Target);
bool Server_CastSpell_Validate(FGameplayTag SpellTag, FHitResult Target); // never blindly true

UFUNCTION(NetMulticast, Unreliable)
void Multicast_PlayVFX(FGameplayTag SpellTag, FVector Origin); // VFX only, no logic

UFUNCTION(Client, Reliable)
void Client_ShowDamageNumber(float Damage, FVector WorldPos);
```

---

## Class Ownership Model

| Class | Server? | Clients? | Owns |
|---|---|---|---|
| `AMMDGameMode` | Yes | No | Game flow, spawning, win/lose |
| `AMMDGameState` | Yes | Yes (replicated) | Phase, timer, dungeon seed |
| `AMMDPlayerState` | Yes | Yes (replicated) | Health, mana, spells, inventory |
| `AMMDPlayerCharacter` | Yes | Yes (replicated) | Movement, mesh, ASC proxy |
| `AMMDPlayerController` | Yes | Owning only | Input, camera, local HUD |
| `AMMDEnemyBase` | Yes | No (visual state replicated) | AI, health |

**GameMode is server-only.** Never access it from client code.
**PlayerState persists across respawns.** Character does not — do not store persistent data on Character.

---

## UE Memory Model

```cpp
// CORRECT — GC-safe reference
UPROPERTY()
TObjectPtr<UMMDAbilitySystemComponent> AbilitySystemComponent;

// CORRECT — weak reference (won't prevent GC)
TWeakObjectPtr<AMMDPlayerCharacter> CachedCharacter;

// WRONG — raw pointer to UObject, GC will silently collect it
UMMDAbilitySystemComponent* AbilitySystemComponent; // DO NOT DO THIS

// CORRECT — create UObjects
auto* ASC = NewObject<UMMDAbilitySystemComponent>(this);

// CORRECT — create Actors
auto* Enemy = GetWorld()->SpawnActor<AMMDEnemyBase>(EnemyClass, SpawnTransform);

// WRONG — new/delete for UObjects
auto* ASC = new UMMDAbilitySystemComponent(); // DO NOT DO THIS
```

---

## UE Sharp Edges (SWE Background)

**Build times are slow.** 10–30 min full rebuild is normal. Use Live Coding for iteration.
Avoid unnecessary `#include` — use forward declarations. Keep module dependencies minimal.

**UE containers, not STL.** `TArray`, `TMap`, `TSet`, `FString`, `FName`, `FText`.
STL types work but won't replicate and don't integrate with UE memory tracking.

**`FName` for identifiers, `FString` for display, `FText` for localised UI.**
Never use `FString` as a map key or in hot paths.

**Tick is single-threaded.** Everything on the game thread. Heavy work → `AsyncTask` or `FRunnable`.
Disable tick on Actors that don't need it: `PrimaryActorTick.bCanEverTick = false` in constructor.
**EXCEPTION: Never disable tick on PlayerController** — `PlayerTick()` runs `ProcessPlayerInput()`
which routes ALL input. Disabling it silently kills all keyboard/mouse input.

**`BeginPlay` fires after the world exists.** Constructor must not depend on the world.
Always call `Super::BeginPlay()` first, `Super::EndPlay(EEndPlayReason)` last.

**Logging:**
```cpp
UE_LOG(LogMMD, Warning, TEXT("Value: %d"), MyInt);
// Define category in header: DECLARE_LOG_CATEGORY_EXTERN(LogMMD, Log, All);
// Define in one .cpp:        DEFINE_LOG_CATEGORY(LogMMD);
```

**No `GetAllActorsOfClass()` in Tick** — iterates every actor, O(n) every frame. Use registries.

**Build target for PIE:** Always build `MultiMagicDungeonEditor` (editor DLL), not
`MultiMagicDungeon` (standalone .exe). The standalone target is never loaded by PIE.

**Enhanced Input setup (all 3 required):**
1. `SetInputMode(FInputModeGameOnly())` in PlayerController::BeginPlay
2. `AddMappingContext` in PlayerController::SetupInputComponent
3. `BindAction` in Character::SetupPlayerInputComponent
Missing any one = keyboard input silently doesn't reach the pawn.

**GameMode ↔ GameState pairing:**
`AGameMode` requires `AGameState`. `AGameModeBase` requires `AGameStateBase`.
Mixing them causes "not compatible" error that silently breaks player spawning.

**Visual assets belong in Blueprints, not C++:**
Don't hardcode mesh/material/animation paths via ConstructorHelpers. Create a Blueprint
subclass, set visuals in the editor, and load the BP class in GameMode. This is the one
ConstructorHelpers reference to maintain.

---

## GAS Conventions

- `UMMDAbilitySystemComponent` lives on **PlayerState** for persistence.
  Character has a convenience getter that delegates to PlayerState's ASC.
- All stat changes go through **GameplayEffects** — never modify attributes directly.
- Ability costs and cooldowns are enforced **server-side by GAS** — do not re-implement them.
- Gameplay Tags follow `MMD.Category.Subcategory`: `MMD.Spell.Fireball`, `MMD.Status.Frozen`

---

## Single-Responsibility Rule

Each class does one thing. Signs of violation:
- Class name contains "Manager", "Handler", "Helper", "System" without a clear noun
- Class has both gameplay logic AND UI/VFX logic
- `BeginPlay` is longer than ~30 lines

When in doubt: split. Small classes are easier to replicate, test, and reason about over a network.

---

## What Belongs in Blueprints

| Belongs in BP | Belongs in C++ |
|---|---|
| Skeletal/Static mesh assignments | All gameplay logic |
| Animation blueprint references | Replication |
| VFX / particle setup | Ability activation |
| Sound cue references | Damage / healing |
| Material assignments | AI decision making |
| Level layout / art | Game state changes |
| UI widget layout | Component creation / logic |
| Data Asset values | Input bindings |

If a Blueprint would need to call an RPC or check `HasAuthority()` — it's in the wrong place.

---

## Commit Message Format

```
type(TASK-NNN): short imperative description

Body: what changed and why (not how). Reference task file.

Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>
```

Types: `feat` `fix` `refactor` `test` `docs` `chore`

---

## Absolute Don'ts

- No gameplay authority logic in Blueprints
- No raw `UObject*` pointers without `UPROPERTY()`
- No `GetAllActorsOfClass()` outside editor/init code
- No `Tick()` work that could be event-driven
- No Steam SDK (`OnlineSubsystemSteam`) before Phase 5
- No committing `Binaries/` `Intermediate/` `Saved/` `DerivedDataCache/`
- No `--no-verify` on commits
- No force-pushing `master`
