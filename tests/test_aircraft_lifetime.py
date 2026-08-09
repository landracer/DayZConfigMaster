# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for aircraft lifetime normalization."""

import re

import pytest
from pathlib import Path

from unittest.mock import patch, MagicMock

from dayzconfigmaster.economy.aircraft_lifetime import (
    MAX_VEHICLE_LIFETIME,
    AircraftLifetimeResult,
    AircraftMergeResult,
    AircraftImportResult,
    ensure_aircraft_lifetime,
    ensure_aircraft_types_in_db,
    ensure_rffsheli_types_in_db,
    import_missing_aircraft_classes_to_db,
    discover_aircraft_classes_from_script_logs,
    remove_bogus_vehicle_spawns,
    repair_vehicle_spawn_usages,
    _is_aircraft,
)


SAMPLE_TYPES = """<?xml version="1.0" encoding="UTF-8"?>
<types>
    <type name="RFFS_MH6M">
        <nominal>10</nominal>
        <lifetime>3</lifetime>
        <restock>1800</restock>
        <min>5</min>
    </type>
    <type name="ExpansionHelicopterUh1h">
        <nominal>5</nominal>
        <lifetime>3600</lifetime>
        <restock>1800</restock>
        <min>2</min>
    </type>
    <type name="C130J">
        <nominal>1</nominal>
        <lifetime>3888000</lifetime>
        <restock>3600</restock>
        <min>1</min>
    </type>
    <type name="OffroadHatchback">
        <nominal>20</nominal>
        <lifetime>604800</lifetime>
        <restock>1800</restock>
        <min>10</min>
    </type>
    <type name="AKM">
        <nominal>30</nominal>
        <lifetime>7200</lifetime>
        <restock>0</restock>
        <min>15</min>
    </type>
</types>
"""


@pytest.fixture
def mission_db(tmp_path: Path) -> Path:
    db_dir = tmp_path / "db"
    db_dir.mkdir()
    types_path = db_dir / "types.xml"
    types_path.write_text(SAMPLE_TYPES, encoding="utf-8")
    return types_path


def test_ensure_aircraft_lifetime_updates_short_lifetimes(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db)

    assert result.success
    assert result.changed_count == 2
    assert "RFFS_MH6M" in result.changed
    assert "ExpansionHelicopterUh1h" in result.changed
    assert "C130J" in result.skipped
    assert "OffroadHatchback" not in result.changed
    assert "AKM" not in result.changed

    # Verify the file was actually updated.
    content = mission_db.read_text(encoding="utf-8")
    assert re.search(
        rf"<lifetime>\s*{MAX_VEHICLE_LIFETIME}\s*</lifetime>", content
    )
    assert not re.search(r"<lifetime>\s*3\s*</lifetime>", content)
    assert not re.search(r"<lifetime>\s*3600\s*</lifetime>", content)

    # Non-air entries must be untouched.
    assert re.search(r"<lifetime>\s*604800\s*</lifetime>", content)
    assert re.search(r"<lifetime>\s*7200\s*</lifetime>", content)


def test_ensure_aircraft_lifetime_creates_backup(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db)

    assert result.success
    assert result.backup_path is not None
    assert result.backup_path.exists()
    backup_text = result.backup_path.read_text(encoding="utf-8")
    assert re.search(r"<lifetime>\s*3\s*</lifetime>", backup_text)


def test_ensure_aircraft_lifetime_no_change_when_all_max(mission_db: Path):
    # First run normalizes everything.
    ensure_aircraft_lifetime(mission_db)

    # Second run should report no changes.
    result = ensure_aircraft_lifetime(mission_db)
    assert result.success
    assert result.changed_count == 0
    assert "C130J" in result.skipped


def test_ensure_aircraft_lifetime_missing_file(tmp_path: Path):
    missing = tmp_path / "db" / "types.xml"
    result = ensure_aircraft_lifetime(missing)

    assert not result.success
    assert "not found" in result.error
    assert result.changed_count == 0


def test_ensure_aircraft_lifetime_custom_max(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db, max_lifetime=1000)

    assert result.success
    # Only RFFS_MH6M was below the custom cap.
    assert "RFFS_MH6M" in result.changed
    assert "ExpansionHelicopterUh1h" in result.skipped
    assert "C130J" in result.skipped

    content = mission_db.read_text(encoding="utf-8")
    assert re.search(r"<lifetime>\s*1000\s*</lifetime>", content)


def test_result_dataclass_properties():
    result = AircraftLifetimeResult(
        success=True,
        types_path=Path("/tmp/types.xml"),
        changed=["a", "b"],
        skipped=["c"],
    )
    assert result.changed_count == 2
    assert result.skipped_count == 1


def test_root_types_xml_also_normalized(tmp_path: Path):
    """Root types.xml (where mod aircraft often live) must be normalized too."""
    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    mission_dir.mkdir(parents=True)
    root_types = mission_dir / "types.xml"
    root_types.write_text(SAMPLE_TYPES, encoding="utf-8")

    result = ensure_aircraft_lifetime(root_types)
    assert result.success
    assert "RFFS_MH6M" in result.changed
    assert "ExpansionHelicopterUh1h" in result.changed

    content = root_types.read_text(encoding="utf-8")
    assert re.search(
        rf"<lifetime>\s*{MAX_VEHICLE_LIFETIME}\s*</lifetime>", content
    )


def test_merge_aircraft_from_root_to_db(tmp_path: Path):
    """Aircraft classes in root types.xml but missing from db/types.xml are copied."""
    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    mission_dir.mkdir(parents=True)
    db_dir = mission_dir / "db"
    db_dir.mkdir()

    root_types = mission_dir / "types.xml"
    root_types.write_text(SAMPLE_TYPES, encoding="utf-8")

    db_types = db_dir / "types.xml"
    db_types.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="OffroadHatchback">\n'
        '        <nominal>20</nominal>\n'
        '        <lifetime>604800</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>10</min>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    result = ensure_aircraft_types_in_db(mission_dir)

    assert result.success
    assert result.added_count == 3
    assert "RFFS_MH6M" in result.added
    assert "ExpansionHelicopterUh1h" in result.added
    assert "C130J" in result.added

    db_content = db_types.read_text(encoding="utf-8")
    assert '<type name="RFFS_MH6M">' in db_content
    assert '<type name="ExpansionHelicopterUh1h">' in db_content
    assert '<type name="C130J">' in db_content
    # All aircraft entries should have max lifetime.
    assert not re.search(r"<lifetime>\s*3\s*</lifetime>", db_content)
    assert not re.search(r"<lifetime>\s*3600\s*</lifetime>", db_content)


def test_merge_updates_existing_aircraft_lifetime(tmp_path: Path):
    """Aircraft already in db/types.xml get their lifetime maxed."""
    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    mission_dir.mkdir(parents=True)
    db_dir = mission_dir / "db"
    db_dir.mkdir()

    root_types = mission_dir / "types.xml"
    root_types.write_text(SAMPLE_TYPES, encoding="utf-8")

    db_types = db_dir / "types.xml"
    db_types.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="RFFS_MH6M">\n'
        '        <nominal>10</nominal>\n'
        '        <lifetime>1800</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>5</min>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    result = ensure_aircraft_types_in_db(mission_dir)

    assert result.success
    assert result.updated_count == 1
    assert "RFFS_MH6M" in result.updated
    assert result.added_count == 2

    db_content = db_types.read_text(encoding="utf-8")
    assert re.search(
        rf'<type name="RFFS_MH6M">.*?<lifetime>\s*{MAX_VEHICLE_LIFETIME}\s*</lifetime>',
        db_content,
        re.DOTALL,
    )


def test_discover_aircraft_classes_from_script_logs(tmp_path: Path):
    """Aircraft classes printed to script logs are discovered."""
    profiles = tmp_path / "profiles"
    profiles.mkdir()
    log = profiles / "script_2026-08-07_16-01-35.log"
    log.write_text(
        " SCRIPT       : LM_Aircraft\n"
        " SCRIPT       : LM_Helicopters\n"
        " SCRIPT       : LM_MH6\n"
        " SCRIPT       : RFFSHeli_UH1H_Heli\n"
        " SCRIPT       : RFFSHeli_base\n"
        " SCRIPT       : SomeOtherMod\n",
        encoding="utf-8",
    )

    found = discover_aircraft_classes_from_script_logs(profiles)
    assert "LM_MH6" in found
    assert "RFFSHeli_UH1H_Heli" in found
    assert "RFFSHeli_UH1H" in found  # base CfgVehicles class also emitted
    assert "LM_Aircraft" not in found  # filtered as generic base
    assert "RFFSHeli_base" not in found  # filtered as base class
    assert "SomeOtherMod" not in found


def test_import_missing_aircraft_classes_from_logs(tmp_path: Path):
    """Script-discovered aircraft classes are added to db/types.xml."""
    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    mission_dir.mkdir(parents=True)
    db_dir = mission_dir / "db"
    db_dir.mkdir()
    profiles = tmp_path / "profiles"
    profiles.mkdir()

    db_types = db_dir / "types.xml"
    db_types.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="OffroadHatchback">\n'
        '        <lifetime>604800</lifetime>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    log = profiles / "script_2026-08-07_16-01-35.log"
    log.write_text(
        " SCRIPT       : RFFSHeli_UH1H_Heli\n"
        " SCRIPT       : LM_MH6\n",
        encoding="utf-8",
    )

    result = import_missing_aircraft_classes_to_db(mission_dir, profiles)

    assert result.success
    # The script-only _Heli class is skipped; the base CfgVehicles class and
    # the LM class are imported.
    assert result.imported_count == 2
    assert "RFFSHeli_UH1H_Heli" not in result.imported
    assert "RFFSHeli_UH1H" in result.imported
    assert "LM_MH6" in result.imported

    db_content = db_types.read_text(encoding="utf-8")
    assert '<type name="RFFSHeli_UH1H_Heli">' not in db_content
    assert '<type name="RFFSHeli_UH1H">' in db_content
    assert '<type name="LM_MH6">' in db_content
    assert re.search(
        rf'<type name="RFFSHeli_UH1H">.*?<lifetime>\s*{MAX_VEHICLE_LIFETIME}\s*</lifetime>',
        db_content,
        re.DOTALL,
    )


def test_ensure_rffsheli_types_in_db(tmp_path: Path):
    """Official RFFSHeli types.xml entries are merged into db/types.xml."""
    mission_dir = tmp_path / "mpmissions" / "dayzOffline.enoch"
    mission_dir.mkdir(parents=True)
    db_dir = mission_dir / "db"
    db_dir.mkdir()

    db_types = db_dir / "types.xml"
    db_types.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="RFFSHeli_UH1H">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3600</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    official = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'
        '<types>\n'
        '    <type name="RFFSHeli_UH1H">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <quantmin>-1</quantmin>\n'
        '        <quantmax>-1</quantmax>\n'
        '        <cost>100</cost>\n'
        '        <flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>\n'
        '    </type>\n'
        '    <type name="RFFSHeli_UH1H_Wreck">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3600</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '    </type>\n'
        '    <type name="RFFSHeli_wiring_harness">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>28800</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>\n'
        '        <category name="tools"/>\n'
        '        <usage name="Industrial"/>\n'
        '        <tag name="floor"/>\n'
        '    </type>\n'
        '</types>\n'
    )

    mock_response = MagicMock()
    mock_response.read.return_value = official.encode("utf-8")
    mock_response.__enter__ = MagicMock(return_value=mock_response)
    mock_response.__exit__ = MagicMock(return_value=False)

    with patch("dayzconfigmaster.economy.aircraft_lifetime.urlopen", return_value=mock_response):
        result = ensure_rffsheli_types_in_db(mission_dir)

    assert result.success
    assert result.updated_count == 1
    assert "RFFSHeli_UH1H" in result.updated
    assert result.added_count == 2
    assert "RFFSHeli_UH1H_Wreck" in result.added
    assert "RFFSHeli_wiring_harness" in result.added

    db_content = db_types.read_text(encoding="utf-8")
    assert '<type name="RFFSHeli_UH1H_Wreck">' in db_content
    assert '<type name="RFFSHeli_wiring_harness">' in db_content
    assert '<category name="tools"' in db_content
    assert '<usage name="Industrial"' in db_content


def test_is_aircraft_rejects_wrecks_and_parts():
    """Detector must not match wrecks, parts, covers or mod module classes."""
    assert not _is_aircraft("Land_Wreck_hb01_aban2")
    assert not _is_aircraft("statichelicrash")
    assert not _is_aircraft("staticairplanecrate")
    assert not _is_aircraft("wreck_mi8")
    assert not _is_aircraft("ext_mi24_wheel_1")
    assert not _is_aircraft("ext_spraycan_ah64d")
    assert not _is_aircraft("lm_patty_wagon")
    assert not _is_aircraft("rffs_carcover_apache")
    assert not _is_aircraft("rffsheli_core")
    assert not _is_aircraft("rffsheli_uniform_mod")


def test_is_aircraft_accepts_real_aircraft():
    """Detector must still match real aircraft and helicopter classes."""
    assert _is_aircraft("RFFSHeli_UH1H")
    assert _is_aircraft("RFFSHeli_Apache_Heli")
    assert _is_aircraft("LM_A10")
    assert _is_aircraft("LM_MH6")
    assert _is_aircraft("C130J")
    assert _is_aircraft("Harrier")
    assert _is_aircraft("Ultralight")
    assert _is_aircraft("ExpansionHelicopterUh1h")
    assert _is_aircraft("ext_mi8")
    assert _is_aircraft("ext_uh60m")


def test_remove_bogus_vehicle_spawns(tmp_path: Path):
    """Bogus wreck/part/static entries with Town usage are removed."""
    db_types = tmp_path / "db" / "types.xml"
    db_types.parent.mkdir()
    db_types.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="OffroadHatchback">\n'
        '        <nominal>20</nominal>\n'
        '        <lifetime>604800</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>10</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="Land_Wreck_hb01_aban2">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="ext_mi24_wheel_1">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="RFFSHeli_UH1H">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <category name="vehicle"/>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    result = remove_bogus_vehicle_spawns(db_types)

    assert result.success
    assert result.removed_count == 2
    assert "Land_Wreck_hb01_aban2" in result.removed
    assert "ext_mi24_wheel_1" in result.removed

    db_content = db_types.read_text(encoding="utf-8")
    assert '<type name="OffroadHatchback">' in db_content
    assert '<type name="RFFSHeli_UH1H">' in db_content
    assert '<type name="Land_Wreck_hb01_aban2">' not in db_content
    assert '<type name="ext_mi24_wheel_1">' not in db_content
    assert result.backup_path is not None
    assert result.backup_path.exists()


def test_repair_vehicle_spawn_usages(tmp_path: Path):
    """Town usage is stripped from vehicle/air/water and removed from bogus objects."""
    types_path = tmp_path / "types.xml"
    types_path.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="OffroadHatchback">\n'
        '        <nominal>20</nominal>\n'
        '        <lifetime>604800</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>10</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="ModdedCar">\n'
        '        <nominal>10</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>2</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="Land_Wreck_hb01_aban2">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="RFFSHeli_UH1H">\n'
        '        <nominal>0</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>0</min>\n'
        '        <category name="vehicle"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="LittleBird_Heli">\n'
        '        <nominal>5</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>1</min>\n'
        '        <category name="air"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="SpeedBoat">\n'
        '        <nominal>3</nominal>\n'
        '        <lifetime>3888000</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>1</min>\n'
        '        <category name="water"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="AKM">\n'
        '        <nominal>30</nominal>\n'
        '        <lifetime>7200</lifetime>\n'
        '        <restock>0</restock>\n'
        '        <min>15</min>\n'
        '        <category name="weapon"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    result = repair_vehicle_spawn_usages(types_path)

    assert result.success
    assert result.removed_count == 1
    assert "Land_Wreck_hb01_aban2" in result.removed
    assert result.repaired_count == 5
    assert "OffroadHatchback" in result.repaired
    assert "ModdedCar" in result.repaired
    assert "RFFSHeli_UH1H" in result.repaired
    assert "LittleBird_Heli" in result.repaired
    assert "SpeedBoat" in result.repaired

    content = types_path.read_text(encoding="utf-8")
    assert '<type name="OffroadHatchback">' in content
    assert '<type name="ModdedCar">' in content
    assert '<type name="Land_Wreck_hb01_aban2">' not in content
    assert '<type name="RFFSHeli_UH1H">' in content
    assert '<type name="LittleBird_Heli">' in content
    assert '<type name="SpeedBoat">' in content
    assert '<type name="AKM">' in content
    # Vehicle/air/water entries must no longer carry Town usage; weapons keep it.
    assert '<type name="AKM">\n' in content
    assert content.count('usage name="Town"') == 1
    assert result.backup_path is not None
    assert result.backup_path.exists()


def test_repair_vehicle_spawn_usages_fixes_gear_categorised_mod_vehicles(
    tmp_path: Path,
):
    """Mod vehicles written as category=gear + usage=Town are re-categorised."""
    types_path = tmp_path / "types.xml"
    types_path.write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n<types>\n'
        '    <type name="Jeep_GladiatorF9_BOSS_camo_blue">\n'
        '        <nominal>1</nominal>\n'
        '        <lifetime>7200</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>1</min>\n'
        '        <category name="gear"/>\n'
        '        <usage name="Town"/>\n'
        '        <value name="Tier12"/>\n'
        '    </type>\n'
        '    <type name="ToyotaRunner_black">\n'
        '        <nominal>1</nominal>\n'
        '        <lifetime>7200</lifetime>\n'
        '        <restock>1800</restock>\n'
        '        <min>1</min>\n'
        '        <category name="gear"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '    <type name="CivilianTent">\n'
        '        <nominal>10</nominal>\n'
        '        <lifetime>7200</lifetime>\n'
        '        <category name="gear"/>\n'
        '        <usage name="Town"/>\n'
        '    </type>\n'
        '</types>\n',
        encoding="utf-8",
    )

    result = repair_vehicle_spawn_usages(types_path)

    assert result.success
    assert result.repaired_count == 2
    assert "Jeep_GladiatorF9_BOSS_camo_blue" in result.repaired
    assert "ToyotaRunner_black" in result.repaired
    assert "CivilianTent" not in result.repaired

    content = types_path.read_text(encoding="utf-8")
    assert '<category name="vehicle"' in content
    assert '<category name="gear"' in content  # CivilianTent stays gear
    assert "Jeep_GladiatorF9_BOSS_camo_blue" in content
    assert "ToyotaRunner_black" in content
    # Civilian tent is legitimate gear+Town and must remain untouched.
    assert "CivilianTent" in content
    assert content.count('usage name="Town"') == 1
