# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.

"""Tests for dayzconfigmaster.economy.types_repair."""

from pathlib import Path

import pytest

from dayzconfigmaster.economy import TypesXml, repair_nominal_values, TypesRepairResult


def _write_types(path: Path, xml: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(xml, encoding="utf-8")


def test_repair_restores_nominal_from_reference(tmp_path: Path) -> None:
    target = tmp_path / "target" / "db" / "types.xml"
    ref = tmp_path / "reference" / "db" / "types.xml"

    _write_types(
        target,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="ak74">
        <nominal>0</nominal>
        <min>0</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
    <type name="car_sedan">
        <nominal>0</nominal>
        <min>0</min>
        <lifetime>3888000</lifetime>
        <category name="vehicle"/>
    </type>
</types>""",
    )

    _write_types(
        ref,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="ak74">
        <nominal>4</nominal>
        <min>2</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    result = repair_nominal_values(target, [ref])
    assert result.success is True
    assert result.repaired_count == 1
    assert result.repaired[0] == ("ak74", ref.name, 4, 2)
    assert result.skipped == ["car_sedan"]

    repaired_xml = TypesXml.from_file(str(target))
    ak74 = repaired_xml.get_type("ak74")
    assert ak74 is not None
    assert ak74.nominal == 4
    assert ak74.min == 2


def test_repair_leaves_nonzero_entries_alone(tmp_path: Path) -> None:
    target = tmp_path / "target" / "db" / "types.xml"
    ref = tmp_path / "reference" / "db" / "types.xml"

    _write_types(
        target,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="m4a1">
        <nominal>1</nominal>
        <min>1</min>
        <lifetime>7200</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    _write_types(
        ref,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="m4a1">
        <nominal>99</nominal>
        <min>99</min>
        <lifetime>7200</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    result = repair_nominal_values(target, [ref])
    assert result.success is True
    assert result.repaired_count == 0

    repaired_xml = TypesXml.from_file(str(target))
    m4a1 = repaired_xml.get_type("m4a1")
    assert m4a1 is not None
    assert m4a1.nominal == 1


def test_repair_uses_fallback_when_references_miss(tmp_path: Path) -> None:
    target = tmp_path / "target" / "db" / "types.xml"
    ref = tmp_path / "reference" / "db" / "types.xml"
    fallback = tmp_path / "reference" / "types.xml"

    _write_types(
        target,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="custom_gun">
        <nominal>0</nominal>
        <min>0</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    _write_types(
        ref,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="other_gun">
        <nominal>5</nominal>
        <min>3</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    _write_types(
        fallback,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="custom_gun">
        <nominal>2</nominal>
        <min>1</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    result = repair_nominal_values(target, [ref], fallback_path=fallback)
    assert result.success is True
    assert result.repaired_count == 1
    assert result.repaired[0][0] == "custom_gun"


def test_repair_creates_backup_when_requested(tmp_path: Path) -> None:
    target = tmp_path / "target" / "db" / "types.xml"
    ref = tmp_path / "reference" / "db" / "types.xml"

    _write_types(
        target,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="ak74">
        <nominal>0</nominal>
        <min>0</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    _write_types(
        ref,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="ak74">
        <nominal>4</nominal>
        <min>2</min>
        <lifetime>28800</lifetime>
        <category name="weapons"/>
    </type>
</types>""",
    )

    result = repair_nominal_values(target, [ref], backup=True)
    assert result.success is True
    assert result.backup_path is not None
    assert result.backup_path.exists()
    assert result.backup_path.name.startswith("types.xml.pre-repair-")


def test_repair_missing_target_returns_error(tmp_path: Path) -> None:
    target = tmp_path / "missing" / "types.xml"
    ref = tmp_path / "reference" / "db" / "types.xml"
    _write_types(
        ref,
        """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<types>
    <type name="ak74"><nominal>4</nominal><min>2</min></type>
</types>""",
    )

    result = repair_nominal_values(target, [ref])
    assert result.success is False
    assert "not found" in result.error.lower()
