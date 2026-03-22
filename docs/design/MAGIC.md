# Magic System Design

_Core design for the composable spell system. Updated 2026-03-21._

---

## Vision

Spells are **instruction sequences**, not fixed abilities. A small set of primitives
compose to create an infinite space of possible spells. The same execution engine
interprets all spells regardless of how they were built.

```
Fire splash:        SUMMON fire → DETONATE
Fireball:           SUMMON fire → SHAPE sphere → LAUNCH forward
Fire wall:          SUMMON fire → SHAPE wall → PLACE cursor
Teleport:           TARGET self → MOVE to(cursor)
Flaming tree spear: TARGET tree → EXTRACT → SHAPE spear → IMBUE fire → LAUNCH forward
                    → ON_HIT: SUMMON fire → DETONATE
```

The system starts with 4 primitives and 1 element. New primitives extend the system
without changing it. ON_HIT enables recursion (spell triggers spell).

---

## Instruction Set

Primitives fall into three categories:

### Source primitives — acquire material
| Primitive | Parameters | Effect |
|---|---|---|
| `SUMMON` | Element tag | Create raw magical material |
| `TARGET` | Actor/point | Select something in the world |
| `EXTRACT` | — | Pull material from the targeted thing |

### Modifier primitives — shape or enhance
| Primitive | Parameters | Effect |
|---|---|---|
| `SHAPE` | Shape tag | Give material a form (sphere, wall, beam, spear) |
| `IMBUE` | Property tag | Layer a property onto the material |

### Terminal primitives — create game effects
| Primitive | Parameters | Effect |
|---|---|---|
| `LAUNCH` | Direction | Propel the shaped material forward (projectile) |
| `DETONATE` | — | Release energy as AoE at current position |
| `PLACE` | Point | Place the shaped material at a world location |
| `MOVE` | Point | Move the target (or self) to a location |

### Meta primitives — composition
| Primitive | Parameters | Effect |
|---|---|---|
| `ON_HIT` | Nested chain | Execute another spell chain on collision |

### Validation rules

Not every primitive can follow every other. The chain must follow a grammar:
1. A chain must start with a **source** primitive (SUMMON or TARGET)
2. **Modifiers** require a source to have been set
3. A chain must end with a **terminal** primitive
4. ON_HIT is only valid after a terminal that creates a moving/placed object

Invalid chains are rejected before execution (no mana spent).

---

## Execution Model

The executor is a **context-register machine**. It maintains a mutable context
that each primitive reads/writes:

```cpp
struct FMMDSpellContext
{
    FGameplayTag Element;       // What it's made of
    FGameplayTag Shape;         // What form it takes
    FVector Origin;             // Where it starts
    FVector Direction;          // Where it's going
    float Power;                // Accumulated strength
    TWeakObjectPtr<AActor> TargetActor;  // Selected world actor
};
```

Processing a chain:
1. Initialize context with caster's position and aim direction
2. For each primitive in order, call its handler to mutate the context
3. Terminal primitive reads final context and returns `FMMDSpellResult`
4. SpellCastAbility creates game effects from the result

The result is a struct describing what to spawn — not the effect itself:

```cpp
struct FMMDSpellResult
{
    EMMDSpellResultType Type;   // Projectile, AoE, Teleport, Placement
    FGameplayTag Element;
    FGameplayTag Shape;
    FVector Origin;
    FVector Direction;
    float Power;
    float Radius;               // For AoE
};
```

---

## GAS Integration

### One ability wraps all spells

`UMMDSpellCastAbility` is a single GameplayAbility. When activated:
1. Receives spell chain as `FGameplayAbilityTargetData` payload
2. Server validates chain grammar
3. Computes mana cost from chain composition (more primitives = more mana)
4. Calls spell executor to get `FMMDSpellResult`
5. Spawns effects: projectile actor, AoE damage, teleport, etc.
6. Applies mana cost as instant GameplayEffect
7. Plays cast animation montage (GAS handles replication)

### AttributeSet

`UMMDAttributeSet` holds: Health, MaxHealth, Mana, MaxMana.
Replaces the raw floats currently on PlayerState.

### AbilitySystemComponent

`UMMDAbilitySystemComponent` lives on PlayerState (persists across respawns).
Character implements `IAbilitySystemInterface` and delegates to PlayerState's ASC.

---

## Input Modalities

All modalities produce the same `FMMDSpellChain`. The execution engine
doesn't know or care how the chain was built.

### Phase 2: Real-time key sequencing
Player presses primitive keys in sequence, building a chain in real time.
A HUD element shows the chain growing. Cast button executes and clears.

```
Q = SUMMON fire       E = SHAPE sphere
LMB = LAUNCH (terminal)   RMB = DETONATE (terminal)
Escape = cancel chain
```

### Phase 3+: Saved spell macros
Bind completed chains to hotbar slots (1-9). Press slot + LMB to cast instantly.
Craft complex spells at leisure, execute them at speed.

### Phase 3+: Spell crafting UI
Visual chain builder: drag primitive tiles into a sequence, preview the result,
save to spellbook.

### Future: Voice commands
"fireball forward" → parser extracts primitives → builds chain → executes.
Natural language maps directly to the spell grammar.

### Future: Typed notation
Text command: `fire.sphere.forward` → parser → chain → execute.

---

## Replication

### Chain building is local-only
Pressing Q/E adds to a local array on `UMMDSpellChainComponent`. No network traffic.
Chain only crosses the network when cast button is pressed (via GAS ability activation).

### Server-authoritative execution
Chain sent as TargetData in ability activation (GAS's built-in client→server RPC).
Server validates, executes, spawns effects. Clients never execute spell logic.

### Projectiles
`AMMDSpellProjectile` is a replicated actor spawned by the server.
Uses `UProjectileMovementComponent`. On hit, server applies damage GameplayEffect.
VFX via Multicast RPC or replicated properties.

---

## Phase 2 Scope (First Implementation)

| What | Included |
|---|---|
| Primitives | SUMMON, SHAPE, LAUNCH, DETONATE |
| Elements | Fire only |
| Shapes | Sphere only (SHAPE) + default blob (no SHAPE) |
| Input | Real-time key sequencing (Q, E, LMB, RMB, Escape) |
| Spells | Fire Splash (Q→RMB), Fire Projectile (Q→E→LMB) |
| GAS | ASC, AttributeSet (Health/Mana), SpellCastAbility, mana cost |
| VFX | Stub/placeholder (filled in via Blueprint subclasses) |
| Animations | Cast montage from CombatMagicAnims pack |

| What | Deferred |
|---|---|
| TARGET, EXTRACT, IMBUE, PLACE, MOVE, ON_HIT | Future primitives |
| Earth, Water, object-targeting | Future elements |
| Wall, beam, spear | Future shapes |
| Hotbar, crafting UI, voice, typing | Future input modalities |
| Balance, progression, discovery | After core works |
