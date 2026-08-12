# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for mod XML integration."""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.mods.integration import ModIntegrationManager


def test_scan_mod_finds_types_fragment():
    with tempfile.TemporaryDirectory() as tmpdir:
        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="TestItem">\n    <nominal>5</nominal>\n  </type>\n'
            '</types>\n'
        )

        mgr = ModIntegrationManager(Path(tmpdir) / "instance")
        fragments = mgr.scan_mod(mod)
        assert len(fragments) == 1
        assert fragments[0].target_mission_path == "db/types.xml"
    print("test_scan_mod_finds_types_fragment: PASSED")


def test_scan_mod_finds_capitalized_types_dir():
    with tempfile.TemporaryDirectory() as tmpdir:
        mod = Path(tmpdir) / "@TestMod"
        (mod / "Types").mkdir(parents=True)
        (mod / "Types" / "custom_types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="CustomItem"/>\n'
            '</types>\n'
        )

        mgr = ModIntegrationManager(Path(tmpdir) / "instance")
        fragments = mgr.scan_mod(mod)
        assert len(fragments) == 1
        assert fragments[0].target_mission_path == "db/types.xml"
    print("test_scan_mod_finds_capitalized_types_dir: PASSED")


def test_scan_mod_follows_symlinked_subdirectories():
    with tempfile.TemporaryDirectory() as tmpdir:
        real_mod = Path(tmpdir) / "RealMod"
        (real_mod / "Types").mkdir(parents=True)
        (real_mod / "Types" / "symlinked_types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="SymlinkedItem"/>\n'
            '</types>\n'
        )

        wrapper = Path(tmpdir) / "@TestMod"
        wrapper.mkdir()
        (wrapper / "Types").symlink_to(real_mod / "Types", target_is_directory=True)

        mgr = ModIntegrationManager(Path(tmpdir) / "instance")
        fragments = mgr.scan_mod(wrapper)
        assert len(fragments) == 1
        assert fragments[0].target_mission_path == "db/types.xml"
    print("test_scan_mod_follows_symlinked_subdirectories: PASSED")


def test_apply_integration_merges_types():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="Sedan_02"/>\n'
            '</types>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="TestCar"/>\n'
            '</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok
        assert (db_dir / "types.xml").exists()
        content = (db_dir / "types.xml").read_text()
        assert "TestCar" in content
        assert "Sedan_02" in content
        assert "Begin integration: @TestMod" in content
    print("test_apply_integration_merges_types: PASSED")


def test_apply_integration_merges_rootless_fragment():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        mission_dir.mkdir(parents=True)
        (mission_dir / "cfgspawnabletypes.xml").write_text(
            '<?xml version="1.0"?>\n<spawnabletypes>\n'
            '  <type name="Sedan_02"/>\n'
            '</spawnabletypes>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        # Some mods ship cfgspawnabletypes.xml as a raw list of <type> blocks
        # without a wrapping <spawnabletypes> root element.
        (mod / "cfgspawnabletypes.xml").write_text(
            '  <type name="TestCar">\n'
            '    <attachments chance="1.00">\n'
            '      <item name="SparkPlug" chance="1.00" />\n'
            '    </attachments>\n'
            '  </type>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok
        content = (mission_dir / "cfgspawnabletypes.xml").read_text()
        assert "TestCar" in content
        assert "SparkPlug" in content
        assert "Begin integration: @TestMod" in content
    print("test_apply_integration_merges_rootless_fragment: PASSED")


def test_apply_integration_avoids_duplicates():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="TestCar"/>\n'
            '</types>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="TestCar"/>\n'
            '</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok
        content = (db_dir / "types.xml").read_text()
        assert content.count("TestCar") == 1
    print("test_apply_integration_avoids_duplicates: PASSED")


def test_apply_integration_is_idempotent():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        mission_dir.mkdir(parents=True)
        (mission_dir / "cfgspawnabletypes.xml").write_text(
            '<?xml version="1.0"?>\n<spawnabletypes>\n'
            '  <type name="Sedan_02"/>\n'
            '</spawnabletypes>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "cfgspawnabletypes.xml").write_text(
            '  <type name="TestCar">\n'
            '    <attachments chance="1.00">\n'
            '      <item name="SparkPlug" chance="1.00" />\n'
            '    </attachments>\n'
            '  </type>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        assert mgr.apply_integration([mod]).ok
        first = (mission_dir / "cfgspawnabletypes.xml").read_text()

        assert mgr.apply_integration([mod]).ok
        second = (mission_dir / "cfgspawnabletypes.xml").read_text()

        assert first == second
        assert second.count("Begin integration: @TestMod") == 1
    print("test_apply_integration_is_idempotent: PASSED")


def test_restore_backups():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        original = '<?xml version="1.0"?>\n<types>\n  <type name="Vanilla"/>\n</types>\n'
        (db_dir / "types.xml").write_text(original)

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="Modded"/>\n</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        mgr.apply_integration([mod])
        assert "Modded" in (db_dir / "types.xml").read_text()

        restore_result = mgr.restore_backups()
        assert restore_result.ok
        assert (db_dir / "types.xml").read_text() == original
    print("test_restore_backups: PASSED")


def test_apply_integration_sanitizes_invalid_comments():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.chernarusplus"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.chernarusplus"
        mission_dir.mkdir(parents=True)
        (mission_dir / "cfgspawnabletypes.xml").write_text(
            '<?xml version="1.0"?>\n<spawnabletypes>\n'
            '  <type name="Sedan_02"/>\n'
            '</spawnabletypes>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        # Mod ships a fragment with an invalid decorative comment.
        (mod / "cfgspawnabletypes.xml").write_text(
            '  <type name="TestCar">\n'
            '    <!-- ------WHEELS------ -->\n'
            '    <attachments chance="1.00">\n'
            '      <item name="SparkPlug" chance="1.00" />\n'
            '    </attachments>\n'
            '  </type>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])
        assert result.ok

        content = (mission_dir / "cfgspawnabletypes.xml").read_text()
        assert "TestCar" in content
        assert "SparkPlug" in content
        # The invalid double-dash comment must have been sanitized.
        assert "------WHEELS------" not in content
        # The resulting file must be valid XML.
        from xml.etree import ElementTree as ET
        ET.fromstring(content)
    print("test_apply_integration_sanitizes_invalid_comments: PASSED")


def test_apply_integration_repairs_broken_types_fragment():
    """A mod types.xml with no root and CRLF-broken attributes is repaired."""
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.enoch"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="Sedan_02"/>\n'
            '</types>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        # Mirror the broken @2512575701 XML/types.xml layout: raw <type>
        # blocks, CRLF endings, and an attribute name split across lines.
        (mod / "types.xml").write_text(
            '\t<type name="GunnerTruck">\r\n'
            '        <lifetime>3888000</lifetime>\r\n'
            '        <flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in\r\n'
            '_player="0" crafted="0" deloot="0"/>\r\n'
            '    </type>\r\n'
            '\t<type name="JLTVWheel">\r\n'
            '        <nominal>0</nominal>\r\n'
            '        <category name="vehiclesparts"/>\r\n'
            '    </type>\r\n',
            encoding="utf-8",
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok, result.errors
        content = (db_dir / "types.xml").read_text()
        assert "GunnerTruck" in content
        assert "JLTVWheel" in content
        # The repaired file must be valid XML.
        from xml.etree import ElementTree as ET
        root = ET.fromstring(content)
        assert root.tag.lower() == "types"
        names = {t.get("name") for t in root.findall("type")}
        assert "GunnerTruck" in names
        assert "JLTVWheel" in names
        # The broken attribute should have been joined back together.
        assert "count_in_player" in content
    print("test_apply_integration_repairs_broken_types_fragment: PASSED")


def test_apply_integration_rejects_unrepairable_xml():
    """A completely malformed mod XML is skipped and reported."""
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.enoch"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="Sedan_02"/>\n</types>\n'
        )

        mod = Path(tmpdir) / "@BadMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<type name="Foo">\n  <unclosed>\n</type>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        # The broken fragment is skipped, so the merge itself succeeds.
        assert result.ok
        assert any("REJECTED" in w for w in result.warnings)
        content = (db_dir / "types.xml").read_text()
        assert "Foo" not in content
    print("test_apply_integration_rejects_unrepairable_xml: PASSED")


def test_apply_integration_adds_ce_references():
    """Merging a fragment adds the required <ce> reference when one already exists."""
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.enoch"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="Sedan_02"/>\n</types>\n'
        )
        # When the mission already uses <ce>, we extend it.
        (mission_dir / "cfgeconomycore.xml").write_text(
            '<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>\n'
            '<economycore>\n'
            '  <classes>\n    <rootclass name="DefaultWeapon" />\n  </classes>\n'
            '  <defaults>\n    <default name="dyn_radius" value="30" />\n  </defaults>\n'
            '  <ce folder="db">\n    <file name="events.xml" type="events" />\n  </ce>\n'
            '</economycore>\n'
        )

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="TestCar"/>\n</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok, result.errors
        core_text = (mission_dir / "cfgeconomycore.xml").read_text()
        assert '<ce folder="db">' in core_text
        assert '<file name="types.xml" type="types" />' in core_text
    print("test_apply_integration_adds_ce_references: PASSED")


def test_apply_integration_does_not_add_ce_when_defaults_used():
    """Stock cfgeconomycore.xml without <ce> sections is left untouched."""
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.enoch"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        (db_dir / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="Sedan_02"/>\n</types>\n'
        )
        original_core = (
            '<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>\n'
            '<economycore>\n'
            '  <classes>\n    <rootclass name="DefaultWeapon" />\n  </classes>\n'
            '  <defaults>\n    <default name="dyn_radius" value="30" />\n  </defaults>\n'
            '</economycore>\n'
        )
        (mission_dir / "cfgeconomycore.xml").write_text(original_core)

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <type name="TestCar"/>\n</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert result.ok, result.errors
        assert (mission_dir / "cfgeconomycore.xml").read_text() == original_core
        assert "TestCar" in (db_dir / "types.xml").read_text()
    print("test_apply_integration_does_not_add_ce_when_defaults_used: PASSED")


def test_apply_integration_validates_final_output():
    """If the merged output is structurally invalid, the backup is restored."""
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir(exist_ok=True)
        (instance_root / 'serverDZ_default.cfg').write_text('template="dayzOffline.enoch"\n')
        mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
        db_dir = mission_dir / "db"
        db_dir.mkdir(parents=True)
        original = (
            '<?xml version="1.0"?>\n<types>\n'
            '  <type name="Sedan_02"/>\n'
            '</types>\n'
        )
        (db_dir / "types.xml").write_text(original)

        mod = Path(tmpdir) / "@TestMod"
        mod.mkdir()
        # A valid fragment that becomes invalid because it duplicates names
        # is not a hard error, so use a fragment with a wrong child tag.
        (mod / "types.xml").write_text(
            '<?xml version="1.0"?>\n<types>\n  <event name="Evil"/>\n</types>\n'
        )

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration([mod])

        assert not result.ok
        assert any("Unexpected top-level element" in e for e in result.errors)
        # The target file was restored to the original content.
        assert (db_dir / "types.xml").read_text() == original
    print("test_apply_integration_validates_final_output: PASSED")


if __name__ == "__main__":
    test_scan_mod_finds_types_fragment()
    test_scan_mod_finds_capitalized_types_dir()
    test_scan_mod_follows_symlinked_subdirectories()
    test_apply_integration_merges_types()
    test_apply_integration_merges_rootless_fragment()
    test_apply_integration_avoids_duplicates()
    test_apply_integration_is_idempotent()
    test_restore_backups()
    test_apply_integration_sanitizes_invalid_comments()
    test_apply_integration_repairs_broken_types_fragment()
    test_apply_integration_rejects_unrepairable_xml()
    test_apply_integration_adds_ce_references()
    test_apply_integration_validates_final_output()
    print("\nAll mod integration tests passed!")