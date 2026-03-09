# MultiMagicDungeon — Agent Onboarding

This file is the entry point for any Claude agent (or human) picking up this project.
Read this fully before touching any code.

---

## What Is This Project

A multiplayer magic dungeon game in Unreal Engine 5.7. The north-star vision:
**Players build dungeons. Other players raid them. Everyone exists in a shared open world.**

Core loop: `Overworld roam → find a player's dungeon entrance → raid it → return to overworld`

Start session-based (2–8 players), evolve to a persistent open world. See `docs/design/VISION.md`.

---

## How to Pick Up Current Work

1. **Read `docs/PROGRESS.md`** — shows the active task(s) and links to their files
2. **Read the active task file** (`docs/tasks/TASK-NNN-*.md`) — has status, acceptance criteria, test plan
3. **Check current phase in `docs/design/ROADMAP.md`** — see what comes before/after
4. **Read `.claude/CLAUDE.md`** — full C++ and networking coding conventions

If no task is active: the next task in `docs/design/ROADMAP.md` needs a task file written
in `docs/tasks/` before any code is touched. Plan before you ship.

---

## Repo Structure

```
MultiMagicDungeon/          ← UE 5.7 C++ project root
├── CLAUDE.md               ← YOU ARE HERE
├── .claude/
│   ├── CLAUDE.md           ← C++ coding conventions, UE rules, sharp edges
│   └── commands/           ← Custom skills: /review /simplify /pre-commit
├── docs/
│   ├── PROGRESS.md         ← Active tasks only (keep short, link to task files)
│   ├── tasks/              ← One .md per task, historical tasks stay here
│   └── design/             ← Permanent decisions: VISION, ARCHITECTURE, ROADMAP
├── Source/MultiMagicDungeon/   ← All game C++ code
│   └── (Core/ Character/ AbilitySystem/ Dungeon/ Enemy/ UI/ — added per phase)
├── Config/                 ← DefaultGame.ini, DefaultEngine.ini etc.
└── tools/
    └── mmd-cli/            ← Git submodule: dschonholtz/mmd-cli (Python dev CLI)
```

**Do not commit:** `Binaries/` `Intermediate/` `Saved/` `DerivedDataCache/` — all gitignored.

---

## Shell Environment — READ THIS FIRST

This is a **Windows machine running bash (Git Bash)**. There is a critical split:

| Task | Use |
|---|---|
| Git, file ops, grep, Python | bash (works fine) |
| Calling `.bat` files (UBT, UAT) | **PowerShell only** — bash cannot handle spaces in the path |
| Running the mmd CLI | PowerShell or bash via `uv run mmd` |

**Compile command (always use this exact form):**
```powershell
powershell -Command "& 'C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat' MultiMagicDungeon Win64 Development -Project='C:\Users\schon\OneDrive\Documents\Unreal Projects\MultiMagicDungeon\MultiMagicDungeon.uproject' -WaitMutex 2>&1 | Select-Object -Last 20"
```
Success: output ends with `Result: Succeeded`

---

## Available CLI — mmd

The `mmd` CLI lives at `tools/mmd-cli/` (git submodule). See its `CLAUDE.md` for full docs.

```bash
cd tools/mmd-cli
uv run mmd build client     # compile via UBT
uv run mmd test run         # run automation tests
uv run mmd logs local -f    # tail log file
```

`uv` manages the Python env automatically — no manual venv activation needed.
Install uv if missing: `winget install astral-sh.uv`

---

## Custom Skills (Slash Commands)

These live in `.claude/commands/` as markdown files. They are **not** system-registered
skills — they must be read and applied manually by the agent. Do not use the Skill tool
to invoke them; read the file and execute the checklist yourself.

| Command file | When to use |
|---|---|
| `.claude/commands/review.md` | After implementing — staff game dev review checklist |
| `.claude/commands/simplify.md` | After review — remove dead code, split classes, kill abstractions |
| `.claude/commands/pre-commit.md` | Before every commit — compile + review + simplify + test + docs |

**The pre-commit gate is mandatory. Never commit without completing it.**
**Read the file. Apply it manually. Do not invoke via the Skill tool.**

---

## Pre-Commit Workflow (Short Form)

1. `mmd build client` — must end `Result: Succeeded`
2. `/review` — must be APPROVE or APPROVE WITH NITS (no blockers)
3. `/simplify` — apply all REMOVE items; log SPLIT items in task file
4. `mmd test run` — all tests pass
5. Update task file status → DONE; update `docs/PROGRESS.md`
6. `git commit` with format: `type(TASK-NNN): short description`
7. `git push`

---

## Naming Conventions

| Pattern | Used for |
|---|---|
| `AMMD` prefix | Actor subclasses (e.g. `AMMDPlayerCharacter`) |
| `UMMD` prefix | UObject / Component subclasses |
| `FMMD` prefix | Structs |
| `BP_MMD_*` | Blueprints |
| `DA_MMD_*` | Data Assets |
| `MMD.Spell.*` | Gameplay Tags |
| `MMD.Status.*` | Gameplay Tags |
| `MMD.Character.*` | Gameplay Tags |

---

## Task File Format

Every task gets a file at `docs/tasks/TASK-NNN-short-name.md` before any code is written.
Copy `docs/tasks/TEMPLATE.md` as the starting point.

Status values: `PLANNED` → `IN_PROGRESS` → `IN_REVIEW` → `DONE` (or `CANCELLED`)

---

## Repos & Tooling

| Thing | Value |
|---|---|
| Game repo | `git@github.com:dschonholtz/MultiMagicDungeon.git` (branch: `master`) |
| CLI repo | `git@github.com:dschonholtz/mmd-cli.git` (branch: `main`) |
| Clone fresh | `git clone --recurse-submodules git@github.com:dschonholtz/MultiMagicDungeon.git` |
| UE engine | `C:\Program Files\Epic Games\UE_5.7` |
| gh CLI | `C:\Program Files\GitHub CLI\gh.exe` |
| SSH key | `~/.ssh/mmd_github` (configured in `~/.ssh/config`) |
| Python | Managed by `uv` inside `tools/mmd-cli/` |

---

## Key Architectural Decisions (Quick Reference)

- All gameplay authority in **C++ only** — Blueprints are for VFX, UI, data assets
- **GAS** (Gameplay Ability System) for all spells — no custom spell logic outside GAS
- **OnlineSubsystemNull** for dev/LAN — do not add Steam SDK until Phase 5
- **Supabase** is the backend database (Phase 4+) — Postgres + auto REST
- **Hetzner VPS** is the server host (Phase 5+) — Ubuntu 22.04
- **Seamless Travel** (`bUseSeamlessTravel=true`) bridges session-based → persistent world
- GameMode: server only. GameState: replicated to all. PlayerState: per-player replicated.

Full networking details: `docs/design/ARCHITECTURE.md`
Full vision: `docs/design/VISION.md`
Full coding rules: `.claude/CLAUDE.md`
