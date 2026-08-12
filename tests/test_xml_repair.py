# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for the mod XML repair/validation helpers."""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.mods.xml_repair import (
    audit_types_xml,
    repair_mod_xml,
    summarize_types_audit,
    validate_cfgeconomycore,
    validate_mission_xml,
)


def test_repair_valid_xml_unchanged():
    text = '<?xml version="1.0"?>\n<types>\n  <type name="Foo"/>\n</types>\n'
    result = repair_mod_xml(text, "types")
    assert result.ok
    assert not result.was_repaired
    assert "<type name=\"Foo\"/>" in result.text


def test_repair_wraps_rootless_fragment():
    text = '  <type name="Foo">\n    <nominal>5</nominal>\n  </type>\n'
    result = repair_mod_xml(text, "types")
    assert result.ok
    assert result.was_repaired
    assert result.text.startswith("<types>")
    assert result.text.endswith("</types>")


def test_repair_joins_broken_attribute_across_lines():
    text = (
        '<type name="Foo">\n'
        '  <flags count_in_cargo="0" count_in\n'
        '_player="0"/>\n'
        '</type>\n'
    )
    result = repair_mod_xml(text, "types")
    assert result.ok, result.error
    assert "count_in_player" in result.text
    assert "\n_player" not in result.text


def test_repair_sanitizes_double_dash_comments():
    text = '<types>\n  <!-- ------BAD------ -->\n  <type name="Foo"/>\n</types>\n'
    result = repair_mod_xml(text, "types")
    assert result.ok
    assert "------BAD------" not in result.text


def test_repair_rejects_unrepairable():
    text = '<type name="Foo">\n  <unclosed>\n</type>\n'
    result = repair_mod_xml(text, "types")
    assert not result.ok
    assert result.error


def test_validate_mission_xml_catches_wrong_root_child():
    text = '<?xml version="1.0"?>\n<types>\n  <event name="Bad"/>\n</types>\n'
    result = validate_mission_xml(text, "types")
    assert not result.ok
    assert any("Unexpected top-level element" in e for e in result.errors)


def test_validate_mission_xml_warns_duplicate_names():
    text = (
        '<?xml version="1.0"?>\n<types>\n'
        '  <type name="Foo"/>\n  <type name="Foo"/>\n'
        '</types>\n'
    )
    result = validate_mission_xml(text, "types")
    assert result.ok
    assert any("Duplicate name" in w for w in result.warnings)


def test_validate_cfgeconomycore_warns_missing_ce():
    text = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
        '<economycore>\n'
        '  <classes><rootclass name="DefaultWeapon"/></classes>\n'
        '  <defaults><default name="dyn_radius" value="30"/></defaults>\n'
        '</economycore>\n'
    )
    result = validate_cfgeconomycore(text)
    assert result.ok
    assert any("No <ce> sections" in w for w in result.warnings)


def test_validate_final_mission_xml_helper(tmp_path: Path):
    from dayzconfigmaster.gui.app import DayzConfigMasterApp

    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    db_dir = mission_dir / "db"
    db_dir.mkdir(parents=True)
    (db_dir / "types.xml").write_text(
        '<?xml version="1.0"?>\n<types>\n  <type name="Foo"/>\n</types>\n'
    )
    (mission_dir / "cfgeconomycore.xml").write_text(
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
        '<economycore>\n'
        '  <classes><rootclass name="DefaultWeapon"/></classes>\n'
        '  <defaults><default name="dyn_radius" value="30"/></defaults>\n'
        '</economycore>\n'
    )

    ok, msgs = DayzConfigMasterApp._validate_final_mission_xml(
        tmp_path, "dayzOffline.enoch"
    )
    assert ok
    assert any("types.xml" in m for m in msgs)
    assert any("cfgeconomycore.xml" in m for m in msgs)


def test_audit_types_xml_counts_weapons_and_zero_nominal():
    text = (
        '<?xml version="1.0"?>\n<types>\n'
        '  <type name="AKM">\n    <nominal>10</nominal>\n  </type>\n'
        '  <type name="M4A1">\n    <nominal>0</nominal>\n  </type>\n'
        '  <type name="Mosin9130"/>\n'
        '  <type name="Sedan_02"/>\n'
        '</types>\n'
    )
    audit = audit_types_xml(text)
    assert audit.total_types == 4
    assert audit.weapon_types == 3
    assert audit.zero_nominal_weapons == 1
    assert audit.missing_nominal_weapons == 1
    assert "AKM" in audit.sample_weapons
    assert "M4A1" in audit.zero_nominal_names
    assert "Mosin9130" in audit.zero_nominal_names


def test_summarize_types_audit_reports_zero_nominal():
    from dayzconfigmaster.mods.xml_repair import TypesAuditResult
    audit = TypesAuditResult(
        total_types=100,
        weapon_types=10,
        zero_nominal_weapons=2,
        missing_nominal_weapons=1,
        sample_weapons=["AKM", "M4A1"],
    )
    summary = summarize_types_audit(audit)
    assert "100 types" in summary
    assert "10 weapon entries" in summary
    assert "2 weapons with nominal=0" in summary
    assert "1 weapons with no <nominal>" in summary
