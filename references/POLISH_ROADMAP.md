# DayzConfigMaster — Polish Roadmap (Baby Steps, Final Push)

> **Purpose:** Finish everything left after the first roadmap. This closes the *priority fixes*
> from code review, finishes the P1 features **to spec**, fixes newly-found correctness bugs,
> and adds the tests/docs that make this shippable.
>
> **Read this first:**
> - Same rules as `COMPLETION_ROADMAP.md` §0 (branch, venv, small commits, **no bare `except:`**, cross-platform guards).
> - Work **top to bottom**. Each task has: *Why → Files → Steps → Verify → Acceptance*.
> - Tick a box **only** when the Verify command actually passes.
> - The reviewer (AI) will re-run every Verify block and re-open anything that doesn't pass.
>
> **Priority legend:** 🔴 correctness bug (breaks real use) · 🟠 missing feature/wiring · 🟡 polish.

---

## Setup for this round
```bash
cd /home/sysadmin/Documents/references/git/DayzConfigMaster
git checkout -b polish-pass        # fresh branch off current work
source .venv/bin/activate          # or create per COMPLETION_ROADMAP §0.3
pip install -e . pytest
```
Add pytest scoping now so tests never pick up the reference projects in `references/git/`.
**File:** `pyproject.toml` — add at the end:
```toml
[tool.pytest.ini_options]
testpaths = ["tests"]
```
**Verify:**
```bash
python3 -m pytest -q   # must NOT try to collect references/git/SteamworksPy/*
```
- [ ] `pytest` collects only `tests/` and passes.

---

# PHASE PP0 — Critical correctness bugs (do first) 🔴

## Task PP0.1 — Fix `set_type()` dropping economic fields (DATA LOSS) 🔴
**Why:** In `dayzconfigmaster/economy/types_xml.py`, `set_type()` rebuilds the `<type>` XML element
but **only writes `value=""` attributes for category/usage/value** and **completely omits
`nominal`, `min`, `lifetime`, `restock`, and the flag *names***. So when you edit a type and save,
those numbers are silently lost, and category/usage/value names become empty. This is the single
most damaging bug in the CE editor.

**File:** `dayzconfigmaster/economy/types_xml.py` — method `set_type()` (around line 210–245).

**Current (broken) tail of the method:**
```python
        type_elem = XElement("type", name=entry.name)
        root.add_child(type_elem)

        for cat in entry.categories:
            type_elem.add_child(XElement("category", value=cat.value))
        for use in entry.usages:
            type_elem.add_child(XElement("usage", value=use.value))
        for val in entry.values:
            type_elem.add_child(XElement("value", value=val.value))
        return self
```

**Steps:**
1. Look at how `_parse_type_entry()` reads the file (a few lines above). Note it reads:
   - economic values as **child elements** with text: `<nominal>5</nominal>`, `<min>`, `<lifetime>`, `<restock>`.
   - category/usage/value **name** from `child.text_content()` and the `value=` attribute.
   So the writer must mirror the reader exactly.
2. Replace the tail of `set_type()` with a version that writes ALL fields:
```python
        type_elem = XElement("type", name=entry.name)
        root.add_child(type_elem)

        # Economic properties are CHILD ELEMENTS with text (mirror the parser).
        for field_name in ("nominal", "min", "lifetime", "restock"):
            value = getattr(entry, field_name, 0)
            child = XElement(field_name)
            child.text = str(int(value))
            type_elem.add_child(child)

        # category/usage/value: name goes in text_content, value in the attribute.
        for cat in entry.categories:
            elem = XElement("category", value=cat.value or "")
            elem.text = cat.name
            type_elem.add_child(elem)
        for use in entry.usages:
            elem = XElement("usage", value=use.value or "")
            elem.text = use.name
            type_elem.add_child(elem)
        for val in entry.values:
            elem = XElement("value", value=val.value or "")
            elem.text = val.name
            type_elem.add_child(elem)
        return self
```
   > If `XElement` uses a different way to set text than `.text` (check `ce_xml.py`), use that.
   > Check whether the parser reads text via `.text` or `.text_content()` and match it.

**Verify (round-trip must preserve numbers):**
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml, TypeEntry
xml = '<types><type name=\"Apple\"><nominal>10</nominal><min>3</min><lifetime>100</lifetime><restock>50</restock><category name=\"food\"/></type></types>'
t = TypesXml.parse(xml)
e = t.get_type('Apple'); assert e.nominal == 10, e.nominal
e.nominal = 99
t.set_type(e)
out = t.to_xml()
t2 = TypesXml.parse(out)
assert t2.get_type('Apple').nominal == 99, 'nominal lost on round-trip!'
assert t2.get_type('Apple').min == 3, 'min lost on round-trip!'
print('round-trip preserves economic fields: OK')
"
```
**Acceptance:**
- [ ] `set_type()` writes `nominal/min/lifetime/restock` as child elements.
- [ ] category/usage/value **names** survive a parse→set→parse round-trip.
- [ ] The Verify script prints the OK line.

---

## Task PP0.2 — Fix wrong default root tag for types.xml 🔴
**Why:** `TypesXml.doc` creates the document root as `"spawnabletypes"` (copy-paste from the
spawnable-types editor). A brand-new types.xml would be written with the wrong root element
`<spawnabletypes>` instead of `<types>`.

**File:** `dayzconfigmaster/economy/types_xml.py` — the `doc` property (around line 80).

**Steps:**
1. Find:
```python
    @property
    def doc(self) -> XDoc:
        if self._doc is None:
            self._doc = XDoc.create("spawnabletypes")
        return self._doc
```
2. Change `"spawnabletypes"` to `"types"`.

**Verify:**
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml, TypeEntry
t = TypesXml()
t.set_type(TypeEntry(name='Apple', nominal=5))
assert t.to_xml().lstrip().startswith('<types'), t.to_xml()[:40]
print('root tag is <types>: OK')
"
```
**Acceptance:**
- [ ] New types documents use `<types>` as the root.

---

## Task PP0.3 — Replace the fake print-only `economy` CLI handlers 🔴
**Why:** `_handle_economy()` in `dayzconfigmaster/cli/handler.py` (around line 1006) does **nothing
real** — `edit`/`list`/`validate` just `print()` a sentence and return `True`. Users think it worked
but no file is touched. (Same pattern exists in `_handle_logs` — see PP4.)

**File:** `dayzconfigmaster/cli/handler.py` — `_handle_economy()`.

**Steps (make each action real):**
1. **`economy list`** — actually load the file and print entries:
```python
elif action == 'list':
    from dayzconfigmaster.economy.types_xml import TypesXml
    from pathlib import Path
    file_arg = getattr(args, 'file', None) or getattr(args, 'type', None)
    # Resolve to a real types.xml path (accept a path or a CE folder)
    path = self._resolve_ce_file(file_arg)   # small helper you add
    if not path or not Path(path).exists():
        print(f"File not found: {file_arg}")
        return False
    types = TypesXml.from_file(str(path))
    if types is None:
        print(f"Failed to parse: {path}")
        return False
    for name, entry in types.get_all_types().items():
        print(f"{entry.name}: nominal={entry.nominal} min={entry.min} "
              f"lifetime={entry.lifetime} restock={entry.restock}")
    return True
```
2. **`economy edit`** — load, set the field, back up, save:
```python
elif action == 'edit':
    from dayzconfigmaster.economy.types_xml import TypesXml
    path = self._resolve_ce_file(args.file)
    types = TypesXml.from_file(str(path))
    if types is None:
        print(f"Failed to load {args.file}")
        return False
    entry = types.get_type(args.key.split('.')[0])  # e.g. "Apple.nominal"
    # decide your key format; simplest: --key Apple --value nominal=99
    # (document whichever format you pick in --help text)
    ...
    types.backup_types()          # snapshot before writing (see PP2.3)
    Path(path).write_text(types.to_xml(), encoding='utf-8')
    print(f"Saved {path}")
    return True
```
   > Keep the key/value format simple and update the `add_argument(... help=...)` text to match.
3. **`economy validate`** — run the real validator over the folder:
```python
elif action == 'validate':
    from pathlib import Path
    folder = Path(args.folder)
    if not folder.exists():
        print(f"Folder not found: {folder}")
        return False
    types_file = folder / "types.xml"
    if types_file.exists():
        from dayzconfigmaster.economy.types_xml import TypesXml
        t = TypesXml.from_file(str(types_file))
        print(f"types.xml: {'OK' if t else 'PARSE ERROR'} "
              f"({len(t.get_all_types()) if t else 0} types)")
    return True
```
4. Add the small helper `_resolve_ce_file(self, arg)` that returns a path: if `arg` is a file that
   exists use it; if it's a folder, append `types.xml`; else return `None`.

**Verify:**
```bash
cd /tmp && mkdir -p ce_test && cat > ce_test/types.xml <<'EOF'
<types><type name="Apple"><nominal>10</nominal><min>2</min><lifetime>100</lifetime><restock>0</restock></type></types>
EOF
cd /home/sysadmin/Documents/references/git/DayzConfigMaster
python3 -m dayzconfigmaster.cli.handler economy list -f /tmp/ce_test/types.xml
# Expect a real line: "Apple: nominal=10 min=2 lifetime=100 restock=0"
python3 -m dayzconfigmaster.cli.handler economy validate -f /tmp/ce_test
```
**Acceptance:**
- [ ] `economy list` prints the actual types from a real file (not a canned sentence).
- [ ] `economy edit` changes a value AND writes the file (with a backup first).
- [ ] `economy validate` reports real parse status.
- [ ] `--help` text matches the key/value format you implemented.

---

### ✅ PP0 checklist
- [ ] PP0.1 set_type preserves all fields
- [ ] PP0.2 root tag fixed
- [ ] PP0.3 economy CLI handlers are real

---

# PHASE PP1 — Finish Workshop upload to spec 🟠🔴

## Task PP1.1 — Fix the "create new item" flow 🔴
**Why:** In `dayzconfigmaster/workshop/uploader.py`, `publish()` handles updates but **cannot create
a new item**. It calls `StartItemUpdate(app_id, 0)`. Steam requires `CreateItem()` first (an async
call whose callback returns a real `PublishedFileId_t`), then `StartItemUpdate(app_id, that_id)`.

**File:** `dayzconfigmaster/workshop/uploader.py`, method `publish()`.

**Reference:** `references/git/SteamWorkshopUploader/Assets/Scripts/SteamWorkshopUploader.cs` (CreateItem →
StartItemUpdate → Set* → SubmitItemUpdate) and `references/git/SteamworksPy/steamworks/interfaces/workshop.py`
(`CreateItem`, `SetItemCreatedCallback`, `RunCallbacks`).

**Steps:**
1. Add a helper that creates an item and waits for the callback to deliver the new id:
```python
def _create_new_item(self, steam, workshop, timeout=30):
    """Create a new workshop item; return (published_file_id or None, error)."""
    result = {"id": None, "error": None, "done": False}

    def _on_created(created):
        # created carries the new PublishedFileId_t and an EResult
        result["id"] = getattr(created, "publishedFileId", None) or created.get("publishedFileId")
        result["done"] = True

    workshop.SetItemCreatedCallback(_on_created)
    workshop.CreateItem(self.app_id, 0)  # 0 = k_EWorkshopFileTypeCommunity

    start = time.time()
    while not result["done"] and time.time() - start < timeout:
        steam.run_callbacks()   # PUMP callbacks — required!
        time.sleep(0.1)

    if not result["done"] or not result["id"]:
        return None, "Timed out waiting for CreateItem callback"
    return int(result["id"]), None
```
   > Field/method names (`SetItemCreatedCallback`, `run_callbacks`, `publishedFileId`) must match your
   > installed SteamworksPy — open `references/git/SteamworksPy/steamworks/interfaces/workshop.py` and copy exactly.
2. In `publish()`, replace the id-selection block so the "new" branch creates first:
```python
if existing_id and req.published_file_id is None:
    target_id = existing_id
elif req.published_file_id is not None:
    target_id = req.published_file_id
else:
    target_id, err = self._create_new_item(steam, workshop)
    if err:
        return False, err, None

update_handle = workshop.StartItemUpdate(self.app_id, target_id)
```
3. Use `target_id` (not `GetSubscribedItems()`) as the final id — see PP1.2.

**Acceptance:**
- [ ] New uploads call `CreateItem` first and obtain a real id via its callback.
- [ ] `StartItemUpdate` is always called with a valid id (never 0).

---

## Task PP1.2 — Fix new-ID retrieval + pump callbacks during submit 🔴
**Why:** After submitting, the code reads `GetSubscribedItems()[0]` — that's an unrelated subscribed
item, not the one just uploaded. Also `SubmitItemUpdate` completes via a callback that must be pumped.

**File:** `dayzconfigmaster/workshop/uploader.py`, end of `publish()`.

**Steps:**
1. Track the id you already know (`target_id` from PP1.1). Do **not** call `GetSubscribedItems()`.
2. In the progress-poll loop, pump callbacks each iteration:
```python
while time.time() - start_time < max_wait:
    steam.run_callbacks()                       # add this line
    status = workshop.GetItemUpdateProgress(update_handle)
    ...
```
3. At the end, return `target_id`:
```python
self._save_metadata(req.content_folder, target_id)
verb = "updated" if (existing_id or req.published_file_id) else "created"
return True, f"Workshop item {verb}: {target_id}", target_id
```
4. Delete the `GetSubscribedItems()` block entirely.

**Verify (no Steam needed — validation path still guards):**
```bash
python3 -c "
import inspect, dayzconfigmaster.workshop.uploader as u
src = inspect.getsource(u)
assert 'GetSubscribedItems' not in src, 'still using wrong id source'
assert 'run_callbacks' in src, 'callbacks not pumped'
assert 'CreateItem' in src, 'new-item creation missing'
print('uploader flow fixed: OK')
"
```
**Acceptance:**
- [ ] `GetSubscribedItems` no longer used for the returned id.
- [ ] `run_callbacks()` is pumped in both the create wait and the submit poll.
- [ ] Returned id equals the item actually uploaded.

---

## Task PP1.3 — Add the `workshop_upload` MCP tool 🟠
**Why:** The roadmap required the uploader to be reachable over MCP. Today `MCPWorkshopMethod`
(`dayzconfigmaster/mcp/server.py`, ~line 265) only supports `download`/`update`.

**File:** `dayzconfigmaster/mcp/server.py` — class `MCPWorkshopMethod`.

**Steps:**
1. Add `"upload"` to the `action` enum in `params_schema()` and add properties:
   `content_folder`, `title`, `description`, `preview_file`, `tags` (array), `visibility` (int),
   `change_note`, `published_file_id` (int).
2. In `execute()`, add a branch:
```python
elif action == "upload":
    from dayzconfigmaster.workshop.uploader import WorkshopUploader, WorkshopUploadRequest
    req = WorkshopUploadRequest(
        content_folder=params["content_folder"],
        title=params.get("title", ""),
        description=params.get("description", ""),
        preview_file=params.get("preview_file", ""),
        tags=params.get("tags", []),
        visibility=int(params.get("visibility", 0)),
        change_note=params.get("change_note", ""),
        published_file_id=params.get("published_file_id"),
    )
    ok, msg, fid = WorkshopUploader().publish(req)
    return {"success": ok, "message": msg, "published_file_id": fid}
```

**Verify:**
```bash
python3 -c "
from dayzconfigmaster.mcp.server import MCPWorkshopMethod
m = MCPWorkshopMethod('.')
assert 'upload' in m.params_schema()['properties']['action']['enum']
print('MCP workshop_upload registered: OK')
"
```
**Acceptance:**
- [ ] `upload` appears in the MCP workshop schema enum.
- [ ] The branch validates via the uploader and returns `{success, message, published_file_id}`.

---

### ✅ PP1 checklist
- [ ] PP1.1 create-new flow fixed
- [ ] PP1.2 id retrieval + callback pump fixed
- [ ] PP1.3 MCP upload tool added

---

# PHASE PP2 — Finish CE editing features to spec 🟠

## Task PP2.1 — Make `import_classnames` actually create items 🟠
**Why:** The current method only parses/dedupes names and returns a list — it never creates the
type entries or skips existing ones (the roadmap required create + skip-existing).

**File:** `dayzconfigmaster/economy/types_xml.py` — `import_classnames()`.

**Steps:**
1. Rename the current parse-only logic to a private helper `_parse_classnames(text)` returning the
   ordered unique list (keep it — it's useful and tested).
2. Make `import_classnames` create items:
```python
def import_classnames(self, text: str, defaults: dict = None) -> list:
    """Create type entries from a blob of class names. Skips names that already exist.
    Returns the list of names that were ADDED."""
    defaults = defaults or {}
    added = []
    for name in self._parse_classnames(text):
        if self.get_type(name) is not None:
            continue
        entry = TypeEntry(
            name=name,
            nominal=defaults.get("nominal", 0),
            min=defaults.get("min", 0),
            lifetime=defaults.get("lifetime", 0),
            restock=defaults.get("restock", 0),
        )
        self.set_type(entry)
        added.append(name)
    return added
```

**Verify:**
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml
t = TypesXml.parse('<types><type name=\"Apple\"></type></types>')
added = t.import_classnames('Apple, Banana Cherry\nBanana')
assert added == ['Banana','Cherry'], added   # Apple skipped, Banana deduped
assert t.get_type('Banana') is not None
print('import_classnames creates + skips: OK')
"
```
**Acceptance:**
- [ ] Existing names are skipped.
- [ ] New names become real `TypeEntry` objects in the document.
- [ ] Returns only the names actually added.

---

## Task PP2.2 — Fix `batch_scale` double-write 🟡
**Why:** `batch_scale` manually edits the `<type>` element **and** calls `set_type(entry)`, which
rebuilds the element. With PP0.1 fixed, the manual edit is redundant and can cause duplicate/mismatched
nodes.

**File:** `dayzconfigmaster/economy/types_xml.py` — `batch_scale()` (and check `batch_set_category`,
`batch_add_flag` for the same pattern).

**Steps:**
1. In each batch method, **delete the inline `root = self.doc.root ... elem.add_child(...)` block**.
2. Keep only: update the `entry` dataclass fields, then call `self.set_type(entry)` once. Example
   for `batch_scale`:
```python
current_val = getattr(entry, field, 0) or 0
new_val = int(round(current_val * factor))
if field in caps:
    new_val = min(new_val, caps[field])
setattr(entry, field, max(0, new_val))
self.set_type(entry)          # single source of truth for XML
results[name] = True
```

**Verify:**
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml
t = TypesXml.parse('<types><type name=\"Apple\"><nominal>10</nominal></type></types>')
t.batch_scale(['Apple'], 'nominal', 200)
out = t.to_xml()
assert out.count('<nominal') == 1, 'duplicate nominal element!'
assert TypesXml.parse(out).get_type('Apple').nominal == 20
print('batch_scale single clean write: OK')
"
```
**Acceptance:**
- [ ] No batch method hand-edits XML anymore; all go through `set_type`.
- [ ] No duplicate child elements after a batch op (verify count == 1).

---

## Task PP2.3 — Backups: prune to 20 + undoable restore 🟠
**Why:** `backup_types()` never prunes old backups, and `restore_backup()` doesn't snapshot the
current file first (so a restore can't be undone). The roadmap required newest-20 + undoable restore.

**File:** `dayzconfigmaster/economy/types_xml.py` — `backup_types()`, `restore_backup()`, and add
`list_backups()`.

**Steps:**
1. In `backup_types()`, after writing the new backup, prune:
```python
backups = sorted(backup_path.parent.glob(f"{backup_path.stem.split('.')[0]}*{backup_path.suffix}"))
for old in backups[:-20]:
    try:
        old.unlink()
    except OSError:
        pass
```
   > Make sure the glob only matches backup files, not the live file. Prefer a dedicated
   > hidden backup folder like `.dcm-types-backups/` so pruning is unambiguous.
2. Add:
```python
def list_backups(self) -> list:
    """Return backup paths, newest first."""
    ...   # glob the backup folder, sort reverse by name/mtime
```
3. In `restore_backup()`, snapshot the current file **before** overwriting:
```python
def restore_backup(self, backup_path: Path) -> bool:
    try:
        self.backup_types()          # snapshot current so restore is undoable
        content = Path(backup_path).read_text(encoding='utf-8')
        new_types = TypesXml.parse(content)
        self._doc = new_types._doc
        self._types = dict(new_types.get_all_types())
        return True
    except (OSError, ValueError):
        return False
```
   > Replace the broad `except Exception` with specific ones.

**Verify:**
```bash
python3 -c "
from pathlib import Path
import tempfile
from dayzconfigmaster.economy.types_xml import TypesXml
d = Path(tempfile.mkdtemp()); f = d/'types.xml'
f.write_text('<types><type name=\"Apple\"></type></types>')
t = TypesXml.from_file(str(f)); t._last_loaded_path = str(f)
paths = [t.backup_types() for _ in range(25)]
assert len(t.list_backups()) <= 20, len(t.list_backups())
print('prune-to-20 + list_backups: OK')
"
```
**Acceptance:**
- [ ] Only the newest 20 backups are kept.
- [ ] `restore_backup` snapshots current first.
- [ ] `list_backups()` returns newest-first.

---

## Task PP2.4 — Expose batch / import / backups / restore in the CLI 🟠
**Why:** These CE features exist on the class but are unreachable from the CLI.

**File:** `dayzconfigmaster/cli/handler.py` — add parsers under `economy_subparsers` (near line 349)
and handlers in `_handle_economy` (near line 1006).

**Steps:**
1. Add subcommands (mirror the existing `add_parser` style):
```python
# economy batch
economy_batch = economy_subparsers.add_parser('batch', help='Batch-edit types.xml')
economy_batch.add_argument('-f', '--file', required=True, help='types.xml path or CE folder')
economy_batch.add_argument('--op', required=True,
    choices=['scale-nominal','scale-min','scale-lifetime','scale-restock','set-category','add-usage','add-value'])
economy_batch.add_argument('--value', required=True, help='Percent (for scale) or name (for set/add)')
economy_batch.add_argument('--names', nargs='+', required=True, help='Type names to modify')

# economy import-classnames
economy_import = economy_subparsers.add_parser('import-classnames', help='Create types from a name list')
economy_import.add_argument('-f', '--file', required=True, help='types.xml path')
economy_import.add_argument('--text', required=True, help='Comma/space/newline-separated names')

# economy backups / restore
economy_backups = economy_subparsers.add_parser('backups', help='List types.xml backups')
economy_backups.add_argument('-f', '--file', required=True)
economy_restore = economy_subparsers.add_parser('restore', help='Restore a types.xml backup')
economy_restore.add_argument('-f', '--file', required=True, help='Live types.xml path')
economy_restore.add_argument('-b', '--backup', required=True, help='Backup file to restore')
```
2. Add handler branches in `_handle_economy` that load the file, run the method, then **save + backup**:
```python
elif action == 'batch':
    from dayzconfigmaster.economy.types_xml import TypesXml
    from pathlib import Path
    path = self._resolve_ce_file(args.file)
    t = TypesXml.from_file(str(path))
    op = args.op
    if op.startswith('scale-'):
        field = op.split('-', 1)[1]
        t.batch_scale(args.names, field, float(args.value))
    elif op == 'set-category':
        t.batch_set_category(args.names, args.value)
    elif op in ('add-usage', 'add-value'):
        t.batch_add_flag(args.names, op.split('-')[1], args.value)
    t.backup_types()
    Path(path).write_text(t.to_xml(), encoding='utf-8')
    print(f"Batch {op} applied to {len(args.names)} types; saved {path}")
    return True
# ... similar for import-classnames, backups (print list), restore
```

**Verify:**
```bash
python3 -m dayzconfigmaster.cli.handler economy --help | grep -E "batch|import-classnames|backups|restore"
python3 -m dayzconfigmaster.cli.handler economy batch -f /tmp/ce_test/types.xml --op scale-nominal --value 200 --names Apple
python3 -m dayzconfigmaster.cli.handler economy list -f /tmp/ce_test/types.xml   # nominal should be doubled
```
**Acceptance:**
- [ ] `economy batch`, `economy import-classnames`, `economy backups`, `economy restore` all exist and work.
- [ ] Each mutation writes the file and makes a backup first.

---

### ✅ PP2 checklist
- [ ] PP2.1 import_classnames creates items
- [ ] PP2.2 batch double-write removed
- [ ] PP2.3 backups prune + undoable restore
- [ ] PP2.4 CLI wiring for all CE ops

---

# PHASE PP3 — Data-model completeness (types.xml fidelity) 🟡

## Task PP3.1 — Add the missing `TypeEntry` fields
**Why:** Real DayZ `<type>` entries have more than nominal/min/lifetime/restock. Reference
`references/git/DayzServerTools/src/DayzServerTools.Library/Xml/ItemType.cs`. Missing today: `quantmin`,
`quantmax`, `cost`, `tag[]`, and flags `count_in_map`, `count_in_player`, `count_in_cargo`,
`count_in_hoarder`, `crafted`, `deloot`.

**File:** `dayzconfigmaster/economy/types_xml.py`.

**Steps:**
1. Add fields to the `TypeEntry` dataclass:
```python
    quantmin: int = -1
    quantmax: int = -1
    cost: int = 100
    tags: List[str] = field(default_factory=list)
    flags: dict = field(default_factory=lambda: {
        "count_in_map": True, "count_in_player": False, "count_in_cargo": False,
        "count_in_hoarder": False, "crafted": False, "deloot": False,
    })
```
2. Update `_parse_type_entry()` to read `<quantmin>`, `<quantmax>`, `<cost>`, `<tag name="">`,
   and the `<flags .../>` element attributes.
3. Update `set_type()` (from PP0.1) to WRITE all of them back, exactly mirroring the parser.
4. Update `batch_scale` caps if you add quant scaling (optional).

**Verify (full round-trip of a realistic entry):**
```bash
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml
xml = '''<types><type name=\"M4A1\">
<nominal>5</nominal><lifetime>7200</lifetime><restock>0</restock><min>2</min>
<quantmin>-1</quantmin><quantmax>-1</quantmax><cost>100</cost>
<flags count_in_cargo=\"1\" count_in_map=\"1\" count_in_player=\"0\" crafted=\"0\" deloot=\"0\"/>
<category name=\"weapons\"/><usage name=\"Military\"/><value name=\"Tier3\"/><tag name=\"floor\"/>
</type></types>'''
t = TypesXml.parse(xml); e = t.get_type('M4A1')
assert e.cost == 100 and e.flags['count_in_cargo'] is True
out = TypesXml.parse(t.to_xml()).get_type('M4A1')
assert out.cost == 100 and out.flags['count_in_cargo'] is True
print('full type fidelity round-trip: OK')
"
```
**Acceptance:**
- [ ] `TypeEntry` carries quant/cost/tags/flags.
- [ ] All fields survive a parse→write→parse round-trip.

---

# PHASE PP4 — Remove remaining fake handlers 🟠

## Task PP4.1 — Make `_handle_logs` real
**Why:** `_handle_logs` in `cli/handler.py` just prints sentences. There is a real
`dayzconfigmaster/logs/tailer.py` (`LogTailer`) to use.

**Steps:**
1. `logs tail` → use `LogTailer` to print the last N lines of the resolved log.
2. `logs show` → read and print the file (or last N lines).
3. If a log path can't be resolved, return `False` with a clear message.

**Verify:**
```bash
echo -e "line1\nline2\nERROR boom" > /tmp/script.log
python3 -m dayzconfigmaster.cli.handler logs show -f /tmp/script.log | grep boom
```
**Acceptance:**
- [ ] `logs show` prints real file contents.
- [ ] `logs tail` prints the tail via `LogTailer`.

## Task PP4.2 — Sweep for any other print-only stubs
**Steps:**
```bash
grep -n "just report\|For now\|Would \|# Placeholder\|placeholder" dayzconfigmaster/cli/handler.py
```
Fix each handler that only prints without doing work, or open an issue note in the commit message.
**Acceptance:**
- [ ] No CLI handler returns success without performing its action (or it's explicitly documented as read-only).

---

# PHASE PP5 — Tests, config reconciliation, docs 🟡

## Task PP5.1 — Add real tests
**File:** create `tests/test_types_xml.py`, `tests/test_uploader.py`, `tests/test_preflight.py`,
`tests/test_cli_economy.py`.

Cover at minimum:
- **types_xml:** round-trip preserves all fields (PP0.1/PP3.1); `batch_scale` caps + no dup nodes;
  `import_classnames` create+skip; backups prune-to-20; restore undoable.
- **uploader:** `_validate_request` rejects missing folder, >128 title, >8000 desc, oversized preview,
  bad visibility. (No Steam needed.)
- **preflight:** `_check_patch` errors on missing CfgPatches and empty `units[]`; passes on a good config.
- **cli economy:** `economy list` on a temp file prints the entry; `economy batch scale-nominal`
  doubles the value in the saved file.

**Verify:**
```bash
python3 -m pytest -q     # all green, and clearly more than 5 tests now
python3 -m pytest --collect-only -q | tail -1
```
**Acceptance:**
- [ ] At least ~20 tests, all passing.
- [ ] Tests cover every PP0–PP3 fix.

## Task PP5.2 — Reconcile the two config subsystems
**Why:** `config/models.py` (GUI-style) and `economy/*` (CLI-style) can edit the same files two
different ways and drift.

**Steps:**
1. Decide `economy/*` is the **source of truth** for XML (it has the XDoc round-trip).
2. In `gui/config_manager.py` and the GUI tabs, load/save types.xml via `economy.types_xml.TypesXml`
   instead of `config.models`. Keep `config/models.py` only for file types `economy/` doesn't cover
   yet (serverDZ.cfg, cfggameplay.json), or migrate those too.
3. Remove any now-unused duplicate parser paths.

**Verify:**
```bash
# GUI and CLI must produce byte-identical types.xml for the same edit.
grep -rn "TypesXml\|EconomyConfig" dayzconfigmaster/gui/ | head
```
**Acceptance:**
- [ ] GUI and CLI edit types.xml through the **same** `TypesXml` class.
- [ ] No duplicate/competing types.xml parser remains.

## Task PP5.3 — Docs & final sweep
**Steps:**
1. Update `README.md`: install (`pip install -e .`), the new `economy batch/import-classnames/
   backups/restore` commands, `workshop upload`, `github publish/release`, `tools`, `mod preset`.
2. Ensure `requirements.txt` lists everything imported (add `SteamworksPy` if vendored/available).
3. Final grep sweep:
```bash
grep -rn --include='*.py' -E "TODO|FIXME|placeholder|coming soon|not implemented|For now, just|just report" dayzconfigmaster/ | grep -v base_tab.py
```
   Resolve everything except the intentional abstract method in `gui/dayztabs/base_tab.py`.
**Acceptance:**
- [ ] README documents all new commands.
- [ ] requirements.txt is complete.
- [ ] Grep sweep is clean (only the abstract method remains).

---

# FINAL ACCEPTANCE — run this whole block
```bash
cd /home/sysadmin/Documents/references/git/DayzConfigMaster
source .venv/bin/activate

echo "== A. Round-trip preserves economic fields =="
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml
t=TypesXml.parse('<types><type name=\"A\"><nominal>10</nominal><min>3</min></type></types>')
e=t.get_type('A'); e.nominal=99; t.set_type(e)
r=TypesXml.parse(t.to_xml()).get_type('A')
assert r.nominal==99 and r.min==3; print('OK')"

echo "== B. Root tag is <types> =="
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml, TypeEntry
t=TypesXml(); t.set_type(TypeEntry(name='A', nominal=1))
assert t.to_xml().lstrip().startswith('<types'); print('OK')"

echo "== C. Uploader flow fixed =="
python3 -c "
import inspect, dayzconfigmaster.workshop.uploader as u
s=inspect.getsource(u)
assert 'GetSubscribedItems' not in s and 'run_callbacks' in s and 'CreateItem' in s; print('OK')"

echo "== D. MCP upload registered =="
python3 -c "
from dayzconfigmaster.mcp.server import MCPWorkshopMethod
assert 'upload' in MCPWorkshopMethod('.').params_schema()['properties']['action']['enum']; print('OK')"

echo "== E. import_classnames creates + skips =="
python3 -c "
from dayzconfigmaster.economy.types_xml import TypesXml
t=TypesXml.parse('<types><type name=\"Apple\"></type></types>')
assert t.import_classnames('Apple, Banana Banana')==['Banana']; print('OK')"

echo "== F. Economy CLI is real =="
mkdir -p /tmp/ce_test && printf '%s' '<types><type name="Apple"><nominal>10</nominal></type></types>' > /tmp/ce_test/types.xml
python3 -m dayzconfigmaster.cli.handler economy list -f /tmp/ce_test/types.xml | grep -q "Apple: nominal=10" && echo "OK"

echo "== G. No fake/print-only markers =="
grep -rn --include='*.py' -E "For now, just|just report|# Placeholder|coming soon|not implemented" dayzconfigmaster/ | grep -v base_tab.py || echo "CLEAN"

echo "== H. Tests green =="
python3 -m pytest -q
```

**Project is COMPLETE when:**
- [ ] A–F each print `OK`.
- [ ] G prints `CLEAN`.
- [ ] H (pytest) is green with ~20+ tests.
- [ ] Every PP0–PP5 checklist box is truthfully ticked.

---

## Reviewer notes (what the AI will re-check)
1. `set_type` writes **and** reads every field (no data loss) — verified by round-trip.
2. New-item workshop uploads use `CreateItem` + pumped callbacks; no `GetSubscribedItems` id hack.
3. `workshop_upload` MCP tool exists and validates.
4. `import_classnames` creates items; batch ops go only through `set_type` (no dup nodes).
5. Backups prune to 20; restore is undoable.
6. `economy` and `logs` CLI handlers do real work (not print-only).
7. TypeEntry has quant/cost/tags/flags with full round-trip.
8. GUI and CLI share one types.xml parser.
9. Tests cover all of the above and pass; final acceptance block A–H is green/CLEAN.
