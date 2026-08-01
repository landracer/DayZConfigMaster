# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for map_parser module."""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.config.map_parser import (
    MapModClassifier,
    WorkshopMapParser,
    parse_stock_maps,
    parse_workshop_maps,
)


def test_classify_map_by_dot_map():
    """Folder containing .map files should be classified as map."""
    with tempfile.TemporaryDirectory() as tmpdir:
        item = Path(tmpdir) / "123456789"
        item.mkdir()
        (item / "terrain.map").write_text("map data")

        classifier = MapModClassifier()
        assert classifier.classify_workshop_item(item) == "map"
        print("test_classify_map_by_dot_map: PASSED")


def test_classify_map_by_dayzoffline():
    """Folder containing dayzOffline.[name] should be classified as map."""
    with tempfile.TemporaryDirectory() as tmpdir:
        item = Path(tmpdir) / "123456789"
        item.mkdir()
        (item / "dayzOffline.chernarusplus").mkdir()

        classifier = MapModClassifier()
        assert classifier.classify_workshop_item(item) == "map"
        print("test_classify_map_by_dayzoffline: PASSED")


def test_classify_mod_by_config_cpp():
    """Folder with config.cpp and no map references should be classified as mod."""
    with tempfile.TemporaryDirectory() as tmpdir:
        item = Path(tmpdir) / "123456789"
        item.mkdir()
        (item / "config.cpp").write_text("class CfgPatches { class MyMod {}; };")

        classifier = MapModClassifier()
        assert classifier.classify_workshop_item(item) == "mod"
        print("test_classify_mod_by_config_cpp: PASSED")


def test_workshop_map_parser():
    """Parser should return only map items."""
    with tempfile.TemporaryDirectory() as tmpdir:
        map_item = Path(tmpdir) / "111111111"
        map_item.mkdir()
        (map_item / "terrain.map").write_text("map data")

        mod_item = Path(tmpdir) / "222222222"
        mod_item.mkdir()
        (mod_item / "config.cpp").write_text("class CfgPatches {};")

        parser = WorkshopMapParser(tmpdir)
        maps = parser.get_workshop_maps()
        mods = parser.get_workshop_mods()

        assert "111111111" in maps
        assert "222222222" not in maps
        assert "222222222" in mods
        assert "111111111" not in mods
        print("test_workshop_map_parser: PASSED")


def test_parse_stock_maps():
    """Stock maps should be parsed from mpmissions directory."""
    with tempfile.TemporaryDirectory() as tmpdir:
        mpmissions = Path(tmpdir) / "mpmissions"
        mpmissions.mkdir()
        (mpmissions / "dayzOffline.chernarusplus").mkdir()
        (mpmissions / "dayzOffline.enoch").mkdir()

        maps = parse_stock_maps(tmpdir)
        assert "chernarusplus" in maps
        assert "enoch" in maps
        print("test_parse_stock_maps: PASSED")


def test_parse_workshop_maps():
    """Workshop maps should be parsed from content directory."""
    with tempfile.TemporaryDirectory() as tmpdir:
        map_item = Path(tmpdir) / "123456789"
        map_item.mkdir()
        (map_item / "dayzOffline.namalsk").mkdir()

        maps = parse_workshop_maps(tmpdir)
        assert "123456789" in maps
        assert maps["123456789"][1] == "namalsk"
        print("test_parse_workshop_maps: PASSED")


if __name__ == "__main__":
    test_classify_map_by_dot_map()
    test_classify_map_by_dayzoffline()
    test_classify_mod_by_config_cpp()
    test_workshop_map_parser()
    test_parse_stock_maps()
    test_parse_workshop_maps()
    print("\nAll map_parser tests passed!")
