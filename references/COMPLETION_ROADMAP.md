# DayzConfigMaster — Completion Roadmap (Baby Steps Edition)

> **Who this is for:** Anyone, even a first-week developer. Every task is broken into
> tiny steps with the exact file to open, what to type, and how to prove it works.
>
> **How to use this document:**
> 1. Work **top to bottom**. Phases are ordered by priority (P0 first).
> 2. Do **one task at a time**. Check the box `[x]` when the *acceptance criteria* pass.
> 3. After every task, run the **Verify** commands. Do not move on if they fail.
> 4. Commit after each finished task (see "Git rules" below). Small commits = easy review.
>
> **The reviewer (the AI) will check:** that every acceptance box is truthfully ticked,
> that the Verify commands actually pass, and that no new leftover/dead code was added.

---

## 0. Before you touch anything — one-time setup

### 0.1 Golden rules (read once, follow always)
- **Never** delete a file you don't understand. If unsure, ask.
- **Never** commit secrets (API keys, passwords).
- **Never** use a bare `except:`. Always catch a specific error, e.g. `except OSError:`.
- Make **small commits** with clear messages.
- If a Verify step fails, **fix it before moving on**. Do not skip.
- Keep the app **cross-platform** (Windows, Linux, macOS). Always guard OS-specific code with
  `if os.name == 'nt':` (Windows) vs `else:` (Linux/macOS).

### 0.2 Set up your workspace
```bash
cd /home/sysadmin/Documents/references/git/DayzConfigMaster

# Make sure git is clean and you know the starting point
git status
git log --oneline -5

# Create a working branch so main stays safe
git checkout -b completion-roadmap
```

### 0.3 Create a Python virtual environment (isolated dependencies)
```bash
python3 -m venv .venv
source .venv/bin/activate        # Windows: .venv\Scripts\activate
pip install --upgrade pip
pip install -r requirements.txt
```
> If `requirements.txt` is missing a package you need later, add it there (one per line) and re-run `pip install -r requirements.txt`.

### 0.4 Confirm the app starts today
```bash
# The GUI needs a display; this just proves it imports & boots without crashing.
timeout 3 python3 main.py 2>&1 || true
python3 -c "import dayzconfigmaster; print('package imports OK')"
```
**Acceptance for setup:** No import errors printed. You are on branch `completion-roadmap`.

### 0.5 Git rules for every task below
After each task passes its Verify step:
```bash
git add -A
git commit -m "Pxx.y: short description of what you did"
```
Replace `Pxx.y` with the task number (e.g. `P0.1`).

---

# PHASE P0 — Correctness & Cleanup (do these first, they are quick and safe)

## Task P0.1 — Fix the fake "key generation" bug 🔴
**Problem:** `generate_key()` says "Keys generated successfully" but creates **no files**.
That is a lie the rest of the app trusts. We must make it honest.

**File:** `dayzconfigmaster/build/service.py` (around lines 446–470)

**Steps:**
1. Open the file and find the method `def generate_key(self, key_name: ...)`.
2. Read it fully. Notice it never writes the `.biprivatekey` / `.bikey` files.
3. Real DayZ keys are made by `DSCreateKey.exe` (Windows-only DayZ Tools). Since we are
   cross-platform, do this:
   - **If** a `DSCreateKey` executable is configured/available, call it via `subprocess`.
   - **If not**, DO NOT pretend success. Return `False` with a clear message.
4. Replace the body so it looks like this (adapt names to what's already in the class):

```python
def generate_key(self, key_name: Optional[str] = None) -> tuple:
    """Generate a signing key pair. Returns (success, message)."""
    if not self.key_path:
        return False, "No key path configured"

    key_name = key_name or "dayzconfigmaster"
    if not self.key_path.exists():
        self.key_path.mkdir(parents=True, exist_ok=True)

    private_key = self.key_path / f"{key_name}.biprivatekey"
    public_key = self.key_path / f"{key_name}.bikey"

    # Locate DSCreateKey (part of DayZ Tools, Windows only).
    dscreatekey = self._find_dscreatekey()  # implement helper below
    if dscreatekey is None:
        return False, (
            "DSCreateKey not found. Install DayZ Tools and configure its path, "
            "or generate keys manually. No keys were created."
        )

    try:
        result = subprocess.run(
            [str(dscreatekey), key_name],
            cwd=str(self.key_path),
            capture_output=True,
            text=True,
            timeout=60,
        )
    except (OSError, subprocess.SubprocessError) as exc:
        return False, f"Failed to run DSCreateKey: {exc}"

    if result.returncode != 0:
        return False, f"DSCreateKey failed: {result.stderr.strip()}"

    if not private_key.exists() or not public_key.exists():
        return False, "DSCreateKey ran but key files were not produced"

    return True, f"Keys generated: {public_key.name}, {private_key.name}"
```
5. Add the helper method (put it right below `generate_key`):
```python
def _find_dscreatekey(self):
    """Return Path to DSCreateKey executable, or None if unavailable."""
    import shutil
    # 1) explicit config attribute if the class has one
    configured = getattr(self, "dscreatekey_path", None)
    if configured and Path(configured).exists():
        return Path(configured)
    # 2) on PATH
    exe = "DSCreateKey.exe" if os.name == "nt" else "DSCreateKey"
    found = shutil.which(exe)
    return Path(found) if found else None
```
6. Make sure `import subprocess` and `import os` and `from pathlib import Path` are at the top
   of the file (add any that are missing).

**Verify:**
```bash
python3 -c "
from dayzconfigmaster.build.service import *
import inspect, dayzconfigmaster.build.service as s
src = inspect.getsource(s)
assert 'Keys generated successfully' not in src, 'Old fake message still present!'
print('generate_key no longer returns fake success')
"
```
**Acceptance criteria:**
- [ ] `generate_key` returns `False` with a helpful message when `DSCreateKey` is missing.
- [ ] It only returns `True` when the key files actually exist on disk.
- [ ] The string `"Keys generated successfully"` is gone.

---

## Task P0.2 — Remove dead / leftover files from the old project 🧹
**Problem:** These files are leftovers and confuse everyone. One (`setup.py`) is literally a
copy from the old `DayzServerTools` project and points to a folder that doesn't exist.

**Files to delete:**
- `main-bk.py`  (backup of an old main)
- `dzl.py`  (legacy entry point)
- `extract_cpp.py`  (one-off scraping script, not part of the product)
- `extract_and_save.py`  (same)
- `setup.py`  (references non-existent `src/DayzServerTools.Gui/main.py`)

**Steps:**
1. First, prove nothing imports them:
```bash
cd /home/sysadmin/Documents/references/git/DayzConfigMaster
grep -rn --include='*.py' -E "import (dzl|main_bk|extract_cpp|extract_and_save)|from (dzl|extract_cpp)" . | grep -v ".venv" || echo "No imports found — safe to delete"
```
2. If the command prints "No imports found", delete them:
```bash
git rm main-bk.py dzl.py extract_cpp.py extract_and_save.py setup.py
```
   > If any file was still referenced, STOP and report it instead of deleting.
3. We will create a *correct* packaging file in P0.3.

**Verify:**
```bash
ls main-bk.py dzl.py extract_cpp.py extract_and_save.py setup.py 2>&1 | grep -c "No such file" 
# Expect: 5
timeout 3 python3 main.py 2>&1 || true   # still boots
```
**Acceptance criteria:**
- [ ] All 5 files are gone.
- [ ] `python3 main.py` still starts (no import errors).
- [ ] `git status` shows the deletions staged.

---

## Task P0.3 — Add a correct packaging file (`pyproject.toml`)
**Problem:** We removed the broken `setup.py`. Replace it with a modern, correct one.

**File to CREATE:** `pyproject.toml` (repo root)

**Steps:**
1. Create the file with this content (adjust author/URL if needed):
```toml
[build-system]
requires = ["setuptools>=61.0"]
build-backend = "setuptools.build_meta"

[project]
name = "dayzconfigmaster"
version = "0.1.0"
description = "Cross-platform DayZ server & mod configuration master tool"
readme = "README.md"
requires-python = ">=3.8"
dependencies = [
    "requests",
    "GitPython",
    "Pillow",
]

[project.scripts]
dayzconfigmaster = "dayzconfigmaster.cli.handler:main"

[tool.setuptools.packages.find]
include = ["dayzconfigmaster*"]
```
2. Open `dayzconfigmaster/cli/handler.py` and confirm there is a `def main(` function.
   If the entry function has a different name, update the `[project.scripts]` line to match.

**Verify:**
```bash
pip install -e .
dayzconfigmaster --help 2>&1 | head -5   # should print CLI help, not an error
```
**Acceptance criteria:**
- [ ] `pip install -e .` succeeds.
- [ ] `dayzconfigmaster --help` prints usage text.

---

## Task P0.4 — Add the missing `setup/__init__.py`
**Problem:** `dayzconfigmaster/setup/` has no `__init__.py`, so it can't be imported as a package.

**File to CREATE:** `dayzconfigmaster/setup/__init__.py`

**Steps:**
1. Open `dayzconfigmaster/setup/wizard.py` and note the main class name (e.g. `SetupWizard`).
2. Create `dayzconfigmaster/setup/__init__.py` with:
```python
"""Setup wizard package for first-run configuration."""
from .wizard import SetupWizard  # adjust name if different

__all__ = ["SetupWizard"]
```

**Verify:**
```bash
python3 -c "from dayzconfigmaster.setup import SetupWizard; print('setup package OK')"
```
**Acceptance criteria:**
- [ ] The import above prints `setup package OK`.

---

## Task P0.5 — Replace all bare `except:` blocks with specific exceptions
**Problem:** ~17 places hide errors with a bare `except:` (or `except: pass`), so bugs are invisible.

**Files (known offenders):**
- `dayzconfigmaster/build/preflight_rules.py` (lines ~202, 262, 330, 377, 429, 446, 462)
- `dayzconfigmaster/gui/system_tray.py` (lines ~306, 338, 439)
- `dayzconfigmaster/gui/validation_manager.py` (lines ~232, 287, 295)
- `dayzconfigmaster/server/process_controller.py` (lines ~251, 267)
- `dayzconfigmaster/gui/system_tray_linux.py` (line ~115)
- `dayzconfigmaster/mcp/server.py` (line ~624)

**Steps (repeat for each):**
1. Find every bare except:
```bash
grep -rn --include='*.py' -E "except\s*:" dayzconfigmaster/ | grep -v ".venv"
```
2. For each one, decide which error is actually expected. Common choices:
   - File operations → `except OSError:`
   - Parsing → `except (ValueError, KeyError):`
   - Subprocess → `except (OSError, subprocess.SubprocessError):`
   - Truly unknown but must not crash → `except Exception as exc:` **and log it**.
3. Change `except:` to the specific type, and if the block was `pass`, add a log line:
```python
except OSError as exc:
    logger.debug("Ignoring expected error: %s", exc)  # or print(...) if no logger
```
   > If there is no logger in the file, `import logging; logger = logging.getLogger(__name__)` at the top.

**Verify:**
```bash
# This must print NOTHING (zero bare excepts remain):
grep -rn --include='*.py' -E "except\s*:" dayzconfigmaster/ | grep -v ".venv" || echo "CLEAN: no bare excepts"
python3 -c "import dayzconfigmaster; print('still imports')"
```
**Acceptance criteria:**
- [ ] The grep prints `CLEAN: no bare excepts`.
- [ ] The package still imports.
- [ ] No `except Exception:` is silently `pass` — each at least logs.

---

## Task P0.6 — Fix the stale "WPF placeholder" comment on Linux
**Problem:** `system_tray_linux.py` says `pass # Placeholder for WPF implementation`.
WPF is Windows-only .NET; it means nothing on Linux. It's a copy-paste leftover.

**File:** `dayzconfigmaster/gui/system_tray_linux.py` (around line 65, and the header comments)

**Steps:**
1. Open the file. Read the surrounding function.
2. If that branch should do nothing on Linux, replace the placeholder with an honest comment:
```python
# No native tray action needed here on Linux; handled by the GTK/AppIndicator path above.
```
   and remove the word "WPF".
3. Fix the header docstring lines mentioning WPF so they describe the Linux behavior only.

**Verify:**
```bash
grep -n "WPF" dayzconfigmaster/gui/system_tray_linux.py || echo "No WPF references left"
```
**Acceptance criteria:**
- [ ] No "WPF" text remains in the Linux tray file.
- [ ] The function still returns/behaves sensibly (no crash on import).

---

## Task P0.7 — Set up a test harness so future work is verifiable
**Problem:** Only `tests/test_economy.py` exists. We need `pytest` wired up.

**Steps:**
1. Install pytest and add it to requirements:
```bash
pip install pytest
echo "pytest" >> requirements.txt
```
2. Run existing tests to establish a baseline:
```bash
python3 -m pytest -q 2>&1 | tail -20
```
3. If any test fails, note it. Fix only if it's a quick, obvious break; otherwise record it in the
   task's notes for the reviewer.

**Verify:**
```bash
python3 -m pytest -q 2>&1 | tail -5   # tests run (green or documented failures)
```
**Acceptance criteria:**
- [ ] `pytest` runs and collects tests.
- [ ] Baseline pass/fail count is written into your commit message.

---

### ✅ Phase P0 Done — Checklist
- [ ] P0.1 key generation bug fixed
- [ ] P0.2 dead files removed
- [ ] P0.3 pyproject.toml added, `dayzconfigmaster --help` works
- [ ] P0.4 setup/__init__.py added
- [ ] P0.5 no bare excepts remain
- [ ] P0.6 WPF placeholder removed
- [ ] P0.7 pytest runs

Commit and, if you use a remote, open a PR titled **"Phase P0: correctness & cleanup"**.

---

# PHASE P1 — Close the biggest functional gaps

> These are bigger. Each has its own mini-plan. Do them in order.

## Task P1.1 — Steam Workshop UPLOAD / publish (largest missing feature)

**Goal:** Let users publish/update a mod to the Steam Workshop, matching the reference tools
(`SteamWorkshopUploader`, `SteamworksPy`). Today the app can only *download*.

**Background you must read first (10 min):**
- Look at `references/git/SteamworksPy/steamworks/interfaces/workshop.py` — this is the Python API we will
  wrap. Key calls: `CreateItem`, `StartItemUpdate`, `SetItemTitle`, `SetItemDescription`,
  `SetItemContent`, `SetItemPreview`, `SetItemTags`, `SetItemVisibility`, `SubmitItemUpdate`,
  and progress via `GetItemUpdateProgress`.
- Look at `references/git/SteamWorkshopUploader/WorkshopContent/*.workshop.json` for the metadata shape:
  `publishedfileid, contentfolder, previewfile, visibility, title, description, tags, changenote`.

### Step-by-step
1. **Add the dependency.** Decide how to ship SteamworksPy. Simplest: `pip install SteamworksPy`
   if available, otherwise vendor the `steamworks` folder. Add it to `requirements.txt`.
   > SteamworksPy needs the Steam client running and logged in, plus a `steam_appid.txt`
   > containing DayZ's app id `221100` in the working directory.

2. **Create a new file:** `dayzconfigmaster/workshop/uploader.py`
   ```python
   """Steam Workshop upload/publish via SteamworksPy."""
   from __future__ import annotations
   import json
   from pathlib import Path
   from dataclasses import dataclass, field
   from typing import Optional, List

   DAYZ_APP_ID = 221100

   @dataclass
   class WorkshopUploadRequest:
       content_folder: str
       title: str
       description: str = ""
       preview_file: str = ""            # path to png/jpg (< 1 MB)
       tags: List[str] = field(default_factory=list)
       visibility: int = 0               # 0=public,1=friends,2=private
       change_note: str = ""
       published_file_id: Optional[int] = None  # None = create new

   class WorkshopUploader:
       def __init__(self, app_id: int = DAYZ_APP_ID):
           self.app_id = app_id
           self._steam = None

       def _ensure_steam(self):
           if self._steam is None:
               from steamworks import STEAMWORKS
               self._steam = STEAMWORKS()
               self._steam.initialize()
           return self._steam

       def publish(self, req: WorkshopUploadRequest, on_progress=None) -> tuple:
           """Create or update a workshop item. Returns (success, message, file_id)."""
           # 1. validate inputs (folder exists, preview < 1MB, title <=128, desc <=8000)
           # 2. create item if req.published_file_id is None
           # 3. StartItemUpdate -> SetItem* -> SubmitItemUpdate(change_note)
           # 4. poll GetItemUpdateProgress, call on_progress(percent)
           # 5. return result
           raise NotImplementedError  # fill in following the SteamworksPy calls
       ```
   > Fill in `publish()` by following the exact SteamworksPy call order in
   > `references/git/SteamWorkshopUploader/Assets/Scripts/SteamWorkshopUploader.cs` (same sequence).

3. **Add validation** (before any Steam call):
   - `content_folder` exists and is a directory.
   - `preview_file` (if set) exists and is `< 1_048_576` bytes.
   - `len(title) <= 128`, `len(description) <= 8000`.
   - On failure return `(False, "reason", None)` — never call Steam with bad input.

4. **Persist metadata** like the reference: write a `<name>.workshop.json` next to the content
   folder so the `published_file_id` is remembered for future updates.

5. **Wire into the CLI.** In `dayzconfigmaster/cli/handler.py`, find where `workshop` subcommands
   are registered (search for `workshop`). Add:
   - `workshop upload --folder <path> --title <t> [--desc <d>] [--preview <img>] [--tags a,b] [--visibility public|friends|private] [--note <changelog>] [--id <existing_id>]`
   Route it to `WorkshopUploader.publish(...)`.

6. **Wire into MCP.** In `dayzconfigmaster/mcp/server.py`, find `MCPWorkshopMethod`. Add a
   `workshop_upload` tool with the same parameters, returning success/message/file_id.

### Verify
```bash
# Unit-level: validation must reject bad input WITHOUT needing Steam running.
python3 -c "
from dayzconfigmaster.workshop.uploader import WorkshopUploader, WorkshopUploadRequest
u = WorkshopUploader()
ok, msg, fid = u.publish(WorkshopUploadRequest(content_folder='/does/not/exist', title='x'))
assert ok is False, 'should reject missing folder'
print('validation works:', msg)
"
# CLI shows the new command:
dayzconfigmaster workshop --help 2>&1 | grep -i upload
```
### Acceptance criteria
- [ ] `dayzconfigmaster/workshop/uploader.py` exists with `WorkshopUploader.publish()` implemented.
- [ ] Input validation rejects: missing folder, oversized preview, title > 128, desc > 8000.
- [ ] `workshop upload` CLI command exists and calls the uploader.
- [ ] `workshop_upload` MCP tool exists.
- [ ] A `.workshop.json` file is written/updated storing the published_file_id.
- [ ] Uploading twice with the same folder UPDATES (does not duplicate) the item.

---

## Task P1.2 — Central Economy: batch editing (from DayzServerTools)

**Goal:** Add bulk operations to the economy editors. Reference:
`references/git/DayzServerTools/src/DayzServerTools.Application/ViewModels/ItemTypes/ItemTypesViewModel.cs`.

**Where to work:** `dayzconfigmaster/economy/types_xml.py` (and its GUI tab in
`dayzconfigmaster/gui/app.py`, method `_create_spawnable_types_tab` / types tab).

### Batch operations to implement (on a list of selected items)
1. **Adjust nominal by %** — `new = round(old * factor)`.
2. **Adjust min by %**.
3. **Adjust lifetime by %** (cap at `3_888_000`).
4. **Adjust restock by %** (cap at `3_888_000`).
5. **Set category** on all selected.
6. **Bulk add usage flag / value flag / tag** to all selected (no duplicates).

### Step-by-step
1. In `types_xml.py`, add a method group, e.g.:
```python
def batch_scale(self, class_names, field, percent):
    """Scale an integer field by percent (e.g. 120 = +20%) for the given classes."""
    factor = percent / 100.0
    caps = {"lifetime": 3_888_000, "restock": 3_888_000}
    for name in class_names:
        item = self.get(name)               # use your existing lookup
        if item is None:
            continue
        current = getattr(item, field, 0) or 0
        new_val = int(round(current * factor))
        if field in caps:
            new_val = min(new_val, caps[field])
        setattr(item, field, max(0, new_val))
```
2. Add `batch_set_category(class_names, category)` and
   `batch_add_flag(class_names, flag_kind, flag_value)` (flag_kind in {usage, value, tag}).
3. In the GUI tab, add a small "Batch" panel: a dropdown (operation), an input (value), and an
   "Apply to selected" button that reads the treeview's selected rows and calls the method.

### Verify
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import *   # adjust import to real class
# Build a tiny in-memory doc or load a fixture, then:
# t.batch_scale(['Apple'], 'nominal', 200)  and assert nominal doubled
print('write a real assertion here once the class API is known')
"
```
> Also add a proper test in `tests/test_economy_batch.py` that loads a small XML fixture, runs
> `batch_scale`, and asserts the numbers changed correctly and caps were respected.

### Acceptance criteria
- [ ] `batch_scale`, `batch_set_category`, `batch_add_flag` exist and are covered by a test.
- [ ] Lifetime/restock are capped at 3,888,000.
- [ ] Flags are not duplicated when added twice.
- [ ] The GUI has a working "Apply to selected" batch panel.

---

## Task P1.3 — Classname import (free-text → items)

**Goal:** Paste a blob of class names (comma/space/newline separated) and create items.
Reference: `references/git/DayzServerTools/.../Dialogs/ClassnamesImportViewModel.cs` — it uses regex `\w+`.

**File:** add `import_classnames` to `dayzconfigmaster/economy/types_xml.py` (and a GUI dialog).

### Step-by-step
1. Add:
```python
import re

def import_classnames(self, text, defaults=None):
    """Create items from a blob of class names. Skips names that already exist."""
    defaults = defaults or {}
    names = re.findall(r"\w+", text or "")
    added = []
    for name in names:
        if self.get(name) is not None:
            continue
        self.add(name, **defaults)   # use your existing "add item" API
        added.append(name)
    return added
```
2. In the GUI, add an "Import class names…" button that opens a text box and calls this.

### Verify
```bash
python3 -c "
import re
names = re.findall(r'\w+', 'Apple, Banana\nCherry  Apple')
assert names == ['Apple','Banana','Cherry','Apple']
print('regex parse OK')
"
```
### Acceptance criteria
- [ ] `import_classnames` parses comma/space/newline lists.
- [ ] It skips names that already exist (no duplicates).
- [ ] A GUI dialog calls it and refreshes the list.
- [ ] A test in `tests/` covers the parsing + dedupe.

---

## Task P1.4 — types.xml versioned backups + restore (from dzl)

**Goal:** Before saving types.xml, snapshot the old file so edits are undoable.
Reference behavior (dzl): a hidden `.dzl-types-backups/` folder, files named
`types.<timestamp>.xml`, keep the newest **20**, restore snapshots current first.

**File:** `dayzconfigmaster/economy/types_xml.py` (or a new `economy/backups.py` helper).

### Step-by-step
1. Add a helper:
```python
import time, shutil
from pathlib import Path

def _backup_dir(target: Path) -> Path:
    d = target.parent / f".dcm-{target.stem}-backups"
    d.mkdir(exist_ok=True)
    return d

def make_backup(target: Path, keep: int = 20) -> Path:
    """Copy target into its backup folder; prune to newest `keep`."""
    if not target.exists():
        return None
    stamp = time.strftime("%Y%m%d-%H%M%S")
    dest = _backup_dir(target) / f"{target.stem}.{stamp}{target.suffix}"
    shutil.copy2(target, dest)
    backups = sorted(_backup_dir(target).glob(f"{target.stem}.*{target.suffix}"))
    for old in backups[:-keep]:
        old.unlink(missing_ok=True)
    return dest

def list_backups(target: Path):
    return sorted(_backup_dir(target).glob(f"{target.stem}.*{target.suffix}"), reverse=True)

def restore_backup(backup: Path, target: Path):
    """Snapshot current first (so restore is undoable), then restore the chosen backup."""
    make_backup(target)
    shutil.copy2(backup, target)
```
2. Call `make_backup(path)` **just before** you overwrite types.xml in the save routine.
3. Add CLI: `economy backups` (list) and `economy restore <backup-file>`.

### Verify
```bash
python3 -c "
from pathlib import Path
import tempfile, os
from dayzconfigmaster.economy.types_xml import make_backup, list_backups  # adjust
d = Path(tempfile.mkdtemp()); f = d/'types.xml'; f.write_text('<types/>')
b = make_backup(f); assert b.exists()
assert len(list_backups(f)) == 1
print('backup created:', b.name)
"
```
### Acceptance criteria
- [ ] Saving types.xml creates a timestamped backup first.
- [ ] Only the newest 20 backups are kept.
- [ ] `restore_backup` snapshots the current file before restoring.
- [ ] CLI `economy backups` / `economy restore` work.
- [ ] Covered by a test.

---

## Task P1.5 — Finish profile active-state (remove stubs)

**Goal:** Make `get_active_profile()` / `set_active_profile()` real.
**Files:** `dayzconfigmaster/server/profiles.py` (lines ~115, ~120) and
`dayzconfigmaster/server/instance_manager.py` (line ~209 placeholder).

### Step-by-step
1. Decide where the "active profile" name is stored. Simplest: a small JSON file, e.g.
   `<projects_root>/active_profile.json` = `{"active": "MyServer"}`.
2. Implement:
```python
def get_active_profile(self):
    data = self._read_state()          # reads the JSON file, returns {} if missing
    name = data.get("active")
    return self.get_profile(name) if name else None

def set_active_profile(self, name) -> tuple:
    if self.get_profile(name) is None:
        return False, f"Profile '{name}' does not exist"
    self._write_state({"active": name})
    return True, f"Active profile set to {name}"
```
3. In `instance_manager.py:209`, replace the placeholder comment with the real hookup to
   `ProcessController` (or, if not ready, raise a clear `NotImplementedError` with a message —
   **do not** silently pretend it worked).

### Verify
```bash
python3 -c "
# construct the manager against a temp dir, set an unknown profile -> expect failure
print('add a real assertion once constructor args are known')
"
```
### Acceptance criteria
- [ ] `get_active_profile` returns the real active profile (or None).
- [ ] `set_active_profile` rejects unknown names and persists valid ones.
- [ ] No hardcoded `True, "Profile activated"` remains.
- [ ] The `instance_manager.py:209` placeholder is resolved (implemented or honestly errors).

---

### ✅ Phase P1 Done — Checklist
- [ ] P1.1 Workshop upload works (create + update)
- [ ] P1.2 Batch editing works + tested
- [ ] P1.3 Classname import works + tested
- [ ] P1.4 types.xml backups + restore + tested
- [ ] P1.5 Profile active-state real, no stubs

---

# PHASE P2 — Depth features (bring it up to dzl's power)

## Task P2.1 — DayZ Tools wrappers
Reference: `references/git/dayz-labs/src/Dzl.Core/Tools/*`.
Create thin `subprocess` wrappers (each Windows-only tools; guard with `os.name`):
- **Binarize** — `dayzconfigmaster/tools/binarize.py`
- **CfgConvert / DeRap** (unbinarize config.bin → config.cpp) — `dayzconfigmaster/tools/cfgconvert.py`
- **ImageToPAA** batch (PNG/TGA → PAA, with `--recursive`) — `dayzconfigmaster/tools/imagetopaa.py`
- **Tool discovery/launcher** — scan `<DayZ Tools>\Bin`, expose `tools list` / `tools open <key>`.

**Acceptance criteria:**
- [ ] Each wrapper finds its exe or returns a clear "tool not found" error.
- [ ] `tools list` shows present/missing status per tool.
- [ ] CLI + MCP commands added (`convert_paa`, `unbinarize`, `list_tools`, `open_tool`).

## Task P2.2 — Mod project features
Reference: `references/git/dayz-labs/src/Dzl.Core/Projects/*` and `Mods/*`.
- **`new <name>`** — scaffold a mod (config.cpp, `$PBOPREFIX$`, cfgmods skeleton).
- **Mod presets/loadouts** — save/apply/list named sets of enabled mods (JSON in `mod-presets/`).
- **P: junction repair** — `link <name>` recreates a broken P:\ junction (Windows).

**Acceptance criteria:**
- [ ] `new` produces a buildable skeleton mod folder.
- [ ] `modpreset save|apply|list|rm` work and persist as JSON.
- [ ] `link` detects and repairs broken junctions (Windows) / symlinks (Linux/macOS).

## Task P2.3 — GitHub publish / release
Reference: `references/git/dayz-labs/src/Dzl.Core/Git/GitHub.cs` (uses the `gh` CLI).
Extend `dayzconfigmaster/references/git/manager.py`:
- `repo publish <mod> [--public] [--description ...]` — init + create GitHub repo + push.
- `release <mod> <tag> [--notes ...]` — create + push a tag / GitHub release.

**Acceptance criteria:**
- [ ] Both commands detect missing `gh` and error clearly.
- [ ] `release` creates and pushes a tag.

## Task P2.4 — Implement the preflight `_check_patch` rule
**File:** `dayzconfigmaster/build/preflight_rules.py:185`.
Parse the `CfgPatches` class and validate the `units[]` / `requiredAddons[]` entries; emit
findings for missing/empty patches. Replace the `pass` stub with real parsing.

**Acceptance criteria:**
- [ ] `_check_patch` returns real findings on a sample config missing CfgPatches.
- [ ] The TODO comment is removed.
- [ ] Covered by a test with a good and a bad config sample.

---

# PHASE P3 — Polish & consistency

## Task P3.1 — Reconcile the two config subsystems
Right now there are **two** parallel config layers:
- `dayzconfigmaster/config/models.py` (GUI-style, from DayzServerTools)
- `dayzconfigmaster/economy/*` (dzl-style, used by CLI)

They can drift and edit different code paths for the same file. Pick **one** as the source of
truth (recommend `economy/` for XML fidelity) and have the GUI read/write through it.
- [ ] GUI and CLI edit types.xml through the **same** module.
- [ ] No duplicate/competing parser is left for the same file type.

## Task P3.2 — Test coverage
- [ ] Add tests for: config parsing round-trip, batch ops, backups, workshop validation,
      preflight rules, CLI arg parsing.
- [ ] `python3 -m pytest` is green.

## Task P3.3 — Docs & final review
- [ ] Update `README.md`: install (`pip install -e .`), CLI usage, GUI launch, MCP setup.
- [ ] Update `requirements.txt` with every dependency actually imported.
- [ ] Remove any remaining TODO/placeholder found by:
      `grep -rn --include='*.py' -E "TODO|FIXME|placeholder|NotImplemented|coming soon" dayzconfigmaster/`

---

# FINAL ACCEPTANCE — "Is the project done?"

Run this whole block. Everything must pass / be empty where noted.
```bash
cd /home/sysadmin/Documents/references/git/DayzConfigMaster
source .venv/bin/activate

echo "== 1. No bare excepts =="
grep -rn --include='*.py' -E "except\s*:" dayzconfigmaster/ | grep -v ".venv" || echo "CLEAN"

echo "== 2. No leftover TODO/placeholder =="
grep -rn --include='*.py' -E "TODO|FIXME|placeholder|NotImplemented|coming soon|not implemented" dayzconfigmaster/ || echo "CLEAN"

echo "== 3. No dead root files =="
ls main-bk.py dzl.py extract_cpp.py extract_and_save.py setup.py 2>&1 | grep "No such" | wc -l   # expect 5

echo "== 4. Fake key message gone =="
grep -rn "Keys generated successfully" dayzconfigmaster/ || echo "CLEAN"

echo "== 5. Package imports & CLI works =="
python3 -c "import dayzconfigmaster; print('import OK')"
dayzconfigmaster --help >/dev/null && echo "CLI OK"

echo "== 6. New features present =="
python3 -c "from dayzconfigmaster.workshop.uploader import WorkshopUploader; print('uploader OK')"

echo "== 7. Tests green =="
python3 -m pytest -q
```

**The project is considered COMPLETE when:**
- [ ] Sections 1, 2, 4 print `CLEAN`.
- [ ] Section 3 prints `5`.
- [ ] Sections 5 & 6 print their OK messages.
- [ ] Section 7 (pytest) is green.
- [ ] Every phase checklist above is fully ticked.

---

## Notes for the reviewer (the AI)
When reviewing, confirm each of the following was **actually done**, not just checked:
1. `generate_key` no longer returns success without producing files.
2. The 5 dead root files are gone and nothing imported them.
3. Bare `except:` count is zero.
4. Workshop **upload** exists with real validation and is wired to CLI + MCP.
5. Batch editing, classname import, and backups/restore exist **with tests**.
6. Profile active-state stubs are gone.
7. No new leftover references to `DayzServerTools`, `Avalonia`, or `WPF` were introduced.
8. `pytest` passes and `dayzconfigmaster --help` works.
