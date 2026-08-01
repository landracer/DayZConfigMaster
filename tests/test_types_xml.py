# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Unit tests for economy/types_xml.py

"""
Unit tests for the TypesXml parser/editor.

Covers the polish-pass fixes:
- set_type round-trip preserves all economic and metadata fields
- default root tag is <types>
- category/usage/value names use the DayZ `name` attribute
- batch_scale writes a single clean node (no duplicates)
- import_classnames creates new items and skips existing ones (no data loss)
- backups prune to newest 20; restore is undoable
- full type fidelity (quantmin/quantmax/cost/tags/flags) round-trips
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.economy.types_xml import (
    TypesXml, TypeEntry, Category, Usage, Value,
)


def test_root_tag_is_types():
    t = TypesXml()
    t.set_type(TypeEntry(name="Apple", nominal=1))
    assert "<types>" in t.to_xml()[:80]


def test_set_type_preserves_economic_fields():
    xml = ('<types><type name="Apple"><nominal>10</nominal><min>3</min>'
           '<lifetime>100</lifetime><restock>50</restock></type></types>')
    t = TypesXml.parse(xml)
    e = t.get_type("Apple")
    assert e.nominal == 10 and e.min == 3 and e.lifetime == 100 and e.restock == 50
    e.nominal = 99
    t.set_type(e)
    r = TypesXml.parse(t.to_xml()).get_type("Apple")
    assert r.nominal == 99
    assert r.min == 3
    assert r.lifetime == 100
    assert r.restock == 50


def test_category_name_uses_attribute():
    # Real DayZ format stores name in the `name` attribute, not text content.
    xml = '<types><type name="M4A1"><category name="weapons"/></type></types>'
    e = TypesXml.parse(xml).get_type("M4A1")
    assert e.categories and e.categories[0].name == "weapons"


def test_category_usage_value_round_trip():
    xml = ('<types><type name="M4A1">'
           '<category name="weapons"/><usage name="Military"/><value name="Tier3"/>'
           '</type></types>')
    t = TypesXml.parse(xml)
    out = TypesXml.parse(t.to_xml()).get_type("M4A1")
    assert [c.name for c in out.categories] == ["weapons"]
    assert [u.name for u in out.usages] == ["Military"]
    assert [v.name for v in out.values] == ["Tier3"]


def test_full_fidelity_round_trip():
    xml = ('<types><type name="M4A1">'
           '<nominal>5</nominal><lifetime>7200</lifetime><restock>0</restock><min>2</min>'
           '<quantmin>10</quantmin><quantmax>50</quantmax><cost>100</cost>'
           '<flags count_in_cargo="1" count_in_map="1" count_in_player="0" '
           'crafted="0" deloot="0"/>'
           '<category name="weapons"/><usage name="Military"/><value name="Tier3"/>'
           '<tag name="floor"/></type></types>')
    t = TypesXml.parse(xml)
    e = t.get_type("M4A1")
    assert e.cost == 100
    assert e.quantmin == 10 and e.quantmax == 50
    assert e.tags == ["floor"]
    assert e.flags["count_in_cargo"] is True
    assert e.flags["count_in_player"] is False
    out = TypesXml.parse(t.to_xml()).get_type("M4A1")
    assert out.cost == 100
    assert out.quantmin == 10 and out.quantmax == 50
    assert out.tags == ["floor"]
    assert out.flags["count_in_cargo"] is True
    assert out.flags["count_in_player"] is False


def test_batch_scale_no_duplicate_nodes():
    xml = '<types><type name="Apple"><nominal>10</nominal></type></types>'
    t = TypesXml.parse(xml)
    t.batch_scale(["Apple"], "nominal", 200)
    out = t.to_xml()
    assert out.count("<nominal") == 1
    assert TypesXml.parse(out).get_type("Apple").nominal == 20


def test_batch_scale_caps_lifetime():
    xml = '<types><type name="Tent"><lifetime>1000000</lifetime></type></types>'
    t = TypesXml.parse(xml)
    t.batch_scale(["Tent"], "lifetime", 1000)  # would be 10M, capped at 3.888M
    assert t.get_type("Tent").lifetime == 3_888_000


def test_batch_scale_missing_type():
    t = TypesXml.parse('<types></types>')
    results = t.batch_scale(["Nope"], "nominal", 200)
    assert results["Nope"] is False


def test_import_classnames_creates_and_skips():
    t = TypesXml.parse('<types><type name="Apple"></type></types>')
    added = t.import_classnames("Apple, Banana Banana\nCherry")
    assert added == ["Banana", "Cherry"]  # Apple skipped, Banana deduped
    assert t.get_type("Banana") is not None
    assert t.get_type("Cherry") is not None


def test_import_classnames_no_data_loss():
    # Importing a list that includes an existing item must NOT wipe its values.
    t = TypesXml.parse('<types><type name="Apple"><nominal>50</nominal></type></types>')
    added = t.import_classnames("Apple, Banana")
    assert added == ["Banana"]
    assert t.get_type("Apple").nominal == 50


def test_import_classnames_with_defaults():
    t = TypesXml.parse('<types></types>')
    t.import_classnames("Widget", defaults={"nominal": 7, "min": 2})
    e = t.get_type("Widget")
    assert e.nominal == 7 and e.min == 2


def test_remove_type():
    t = TypesXml.parse('<types><type name="Apple"></type></types>')
    assert t.remove_type("Apple") is True
    assert t.get_type("Apple") is None
    assert t.remove_type("Missing") is False


def test_get_type_case_insensitive():
    t = TypesXml.parse('<types><type name="Apple"></type></types>')
    assert t.get_type("apple") is not None
    assert t.get_type("APPLE") is not None


def test_batch_set_category():
    xml = '<types><type name="Apple"><category name="old"/></type></types>'
    t = TypesXml.parse(xml)
    t.batch_set_category(["Apple"], "food")
    out = TypesXml.parse(t.to_xml()).get_type("Apple")
    assert [c.name for c in out.categories] == ["food"]


def test_backups_prune_to_20():
    d = Path(tempfile.mkdtemp())
    f = d / "types.xml"
    f.write_text('<types><type name="Apple"></type></types>')
    t = TypesXml.from_file(str(f))
    t._last_loaded_path = str(f)
    for _ in range(25):
        t.backup_types()
    assert len(t.list_backups()) <= 20


def test_restore_is_undoable():
    d = Path(tempfile.mkdtemp())
    f = d / "types.xml"
    f.write_text('<types><type name="Apple"><nominal>10</nominal></type></types>')
    t = TypesXml.from_file(str(f))
    t._last_loaded_path = str(f)
    backup = t.backup_types()
    # Change current state, then restore -> restore should snapshot current first.
    e = t.get_type("Apple")
    e.nominal = 999
    t.set_type(e)
    before = len(t.list_backups())
    assert t.restore_backup(backup) is True
    assert t.get_type("Apple").nominal == 10
    # restore snapshotted the current (999) state, so backup count grew.
    assert len(t.list_backups()) == before + 1


def test_from_file_missing_returns_none():
    assert TypesXml.from_file("/nonexistent/path/types.xml") is None
