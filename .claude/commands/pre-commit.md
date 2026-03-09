# Pre-Commit Gate

Run this before every commit. Do not commit if any step fails.

## Steps

1. **Compile check**
   Preferred (once Python is installed): `mmd build client`
   Fallback (always works):
   ```powershell
   powershell -Command "& 'C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat' MultiMagicDungeon Win64 Development -Project='C:\Users\schon\OneDrive\Documents\Unreal Projects\MultiMagicDungeon\MultiMagicDungeon.uproject' -WaitMutex 2>&1 | Select-Object -Last 20"
   ```
   Confirm output ends with `Result: Succeeded`. Stop and fix if any errors or new warnings.

2. **Run /review**
   - Execute the full review checklist from `.claude/commands/review.md`
   - If verdict is REQUEST CHANGES: stop, fix blockers, restart this gate

3. **Run /simplify**
   - Execute the simplification pass from `.claude/commands/simplify.md`
   - Apply all REMOVE items immediately
   - Apply SIMPLIFY/RENAME/SPLIT items unless they would change the scope of the current task (log them in the task file for a follow-up instead)

4. **Test verification**
   Preferred (once Python installed): `mmd test run`
   - Confirm PIE 2-player smoke test was run and passed (or note explicitly why it could not be)
   - Confirm all acceptance criteria in the task file are checked off

5. **Docs update**
   - Task file status updated to DONE
   - `docs/PROGRESS.md` active task list updated
   - `docs/PROGRESS.md` recently completed row added
   - `docs/design/ROADMAP.md` task status updated

6. **Commit message**
   - Format: `<type>(<task-id>): <short description>`
   - Types: `feat`, `fix`, `refactor`, `test`, `docs`, `chore`
   - Body: what changed and why (not how)
   - Footer: `Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>`

## Gate result

Only proceed to `git commit` after:
- [ ] Compile: clean
- [ ] Review: APPROVE or APPROVE WITH NITS
- [ ] Simplify: pass applied
- [ ] Tests: passing
- [ ] Docs: updated
