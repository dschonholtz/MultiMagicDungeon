# mmd — MultiMagicDungeon Dev CLI

A thin CLI that wraps all project tooling. Grows organically — commands are added when the phase that needs them starts.

## Setup (one time)

1. Install Python 3.11+ from https://python.org (not the Windows Store version)
2. From this `tools/` directory:
   ```
   pip install -e .
   ```
3. Confirm: `mmd --help`

## Current Commands

```
mmd build client          # compile game client (Win64 Development)
mmd build client --config Shipping
mmd build server          # compile Linux dedicated server (Phase 5)
mmd test run              # run all MMD automation tests
mmd test run MMD.Core     # run a subset
mmd logs local            # tail Saved/Logs/MultiMagicDungeon.log
mmd logs local --no-follow
```

## Adding Commands

Each domain gets its own module in `mmd/`. Register it in `main.py`:
```python
from . import mynewmodule
app.add_typer(mynewmodule.app, name="mycommand")
```

Planned future commands (added when the phase needs them):
- `mmd package` — Phase 5, package client/server
- `mmd deploy` — Phase 5, rsync to Hetzner + restart service
- `mmd logs remote` — Phase 5, SSH tail from Hetzner
- `mmd db migrate` — Phase 4, push Supabase schema
- `mmd db seed` — Phase 4, seed dev data
- `mmd editor <cmd>` — Remote Control API calls (if/when needed)
