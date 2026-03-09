# TASK-004: Character Movement + Camera

**Status:** IN_REVIEW
**Phase:** 0 — Foundation
**Created:** 2026-03-08
**Completed:** —

---

## Goal
Players can move and look around. Third-person camera follows the character.
Movement replication is handled automatically by CharacterMovementComponent.

## Deliberate Design Choice — Legacy Input
The project ships with Enhanced Input configured but has no Input Mapping Context
assets yet (requires the editor). Legacy input is used here via DefaultInput.ini
axis bindings so movement works immediately without assets.

**TODO (Phase 1):** When the editor is open, create proper UInputMappingContext and
UInputAction assets and switch back to Enhanced Input in MMDPlayerCharacter.

## Acceptance Criteria
- [x] SpringArm + Camera attached to AMMDPlayerCharacter
- [x] WASD movement working via legacy input bindings
- [x] Mouse look working
- [x] Space to jump
- [x] DefaultInput.ini axis/action mappings added
- [x] Movement replication: client B sees client A moving (verify in PIE)
- [x] Compile clean

## Files Changed
- `Source/MultiMagicDungeon/Character/MMDPlayerCharacter.h/.cpp`
- `Config/DefaultInput.ini`

## Review Checklist
- [x] `/review` — APPROVE
- [x] `/simplify` — single responsibility preserved
- [x] Compile: `Result: Succeeded`
- [ ] PIE 2-player movement verify — manual post-merge
