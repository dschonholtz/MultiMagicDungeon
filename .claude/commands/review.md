# Game Dev Code Review

You are acting as a staff-level Unreal Engine game developer reviewing a pull request. Review all recently changed files thoroughly. Be specific, cite file:line, and categorize every finding.

## Review Checklist

### Networking & Replication
- [ ] Every state-mutating function on an Actor is guarded with `HasAuthority()` where needed
- [ ] All replicated variables are declared with `UPROPERTY(Replicated)` or `UPROPERTY(ReplicatedUsing=OnRep_X)`
- [ ] `GetLifetimeReplicatedProps` is implemented and includes every replicated property
- [ ] Server RPCs use `UFUNCTION(Server, Reliable/Unreliable, WithValidation)` — Validate() never blindly returns true
- [ ] No gameplay logic in NetMulticast functions — those are VFX/sound only
- [ ] No client-side code making authoritative decisions (health changes, damage, loot)
- [ ] PlayerState used for per-player persistent data; not storing it on Character (Character can be destroyed/respawned)
- [ ] GameMode only on server — no client-accessible data placed there

### Memory & UE Object Model
- [ ] No raw pointers to UObjects — use `UPROPERTY()` or `TWeakObjectPtr`
- [ ] No `new`/`delete` for UObjects — use `NewObject<>()` or `SpawnActor<>()`
- [ ] No UObject stored in plain `TArray` without `UPROPERTY()` (GC will collect it)
- [ ] `BeginPlay` called after `Super::BeginPlay()`; `EndPlay` calls `Super::EndPlay()`
- [ ] No work done in constructors that requires the world to exist — use `BeginPlay`

### Performance
- [ ] No expensive work in `Tick()` — heavy logic moved to timers or event-driven
- [ ] `SetActorTickEnabled(false)` on Actors that don't need tick
- [ ] No `GetAllActorsOfClass()` in Tick or frequently called functions
- [ ] No string operations (`FString`) in hot paths — use `FName` for identifiers
- [ ] Large data structures not copied by value in function signatures — use `const TArray<T>&`

### Code Quality & Single Responsibility
- [ ] Each class has one clear responsibility — no "God" actors that own gameplay + UI + sound
- [ ] No business logic in HUD/Widget classes — they display data, not compute it
- [ ] No Blueprint-callable functions containing server-authoritative logic
- [ ] Magic numbers replaced with named constants or config properties (`UPROPERTY(EditDefaultsOnly)`)
- [ ] No dead code, commented-out blocks, or TODO left in committed files

### GAS (Gameplay Ability System) — when applicable
- [ ] Ability activation validated server-side (cost, cooldown, tags)
- [ ] GameplayEffects used for all stat changes — no direct attribute mutation
- [ ] AbilitySystemComponent on both PlayerState (for persistence) and Character (for convenience)
- [ ] GameplayTags follow `MMD.Category.Subcategory` naming

### Testing
- [ ] New gameplay logic has a corresponding test or explicit note in the task file explaining why it's untestable
- [ ] PIE 2-player smoke test described and passed

### Simplicity
- [ ] No abstraction added for a single use case — three similar lines beats a premature helper
- [ ] No error handling for impossible states
- [ ] No backwards-compatibility shims for code that isn't public API

---

After reviewing, output:

**FINDINGS** — numbered list, each with: category, file:line, severity (BLOCKER / WARN / NIT), description, suggested fix.

**VERDICT** — one of:
- `APPROVE` — no blockers, ship it
- `APPROVE WITH NITS` — no blockers, minor items noted
- `REQUEST CHANGES` — blockers present, do not commit until resolved

If verdict is REQUEST CHANGES, list only the blockers in a short summary at the top.
