# Network Architecture

_Last updated: 2026-03-08_

## Authority Model

| Class | Runs on | Notes |
|---|---|---|
| `AMMDGameMode` | Server only | Game flow, spawning, win conditions |
| `AMMDGameState` | Server + all clients | Phase, timer, dungeon seed |
| `AMMDPlayerState` | Server + owning client | Health, mana, equipped spells |
| `AMMDPlayerCharacter` | Server + all clients | Movement replicated via CMC |
| `AMMDPlayerController` | Server + owning client | Input, camera, HUD |
| `AMMDEnemyBase` | Server only (AI runs) | Visual state replicated |

## Replication Flow

```
Client presses [CAST]
  → Server RPC: Server_CastSpell(SpellTag, TargetData)
    → Server validates (mana, cooldown via GAS)
      → Apply GameplayEffect (damage, cost)
        → OnRep_Health on all clients
          → Update UI, VFX
```

## RPC Pattern Reference

```cpp
// Client → Server
UFUNCTION(Server, Reliable, WithValidation)
void Server_CastSpell(FGameplayTag SpellTag, FHitResult Target);

// Server → All
UFUNCTION(NetMulticast, Unreliable)
void Multicast_PlaySpellVFX(FGameplayTag SpellTag, FVector Origin);

// Server → Owning Client
UFUNCTION(Client, Reliable)
void Client_ShowDamageNumber(float Damage, FVector WorldPos);
```

## Session Architecture

```
Dev/LAN:    Listen Server (one player hosts + plays)
Production: Dedicated Server + Steam/EOS sessions
```

## Latency Considerations
- Projectile spells: client-side prediction + server reconcile
- Melee/AoE: server authoritative, short grace window
- Movement: Unreal CMC handles prediction natively
- Health/Mana: server authoritative, clients show predicted values
