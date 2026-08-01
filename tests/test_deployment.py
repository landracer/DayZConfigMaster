# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for the server deployment module."""

import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.server.deployment import (
    _format_mod_paths,
    _resolve_mod_source,
    _find_bikey_files,
    _create_mod_wrapper,
    deploy_instance,
    generate_start_script,
    generate_systemd_service,
    deploy_all_instances,
)


def test_format_mod_paths():
    assert _format_mod_paths(["CF", "@MasPuertas"]) == "@CF;@MasPuertas"
    assert _format_mod_paths(["CF,MasPuertas"]) == "@CF;@MasPuertas"
    assert _format_mod_paths(["/opt/dayz/@CF"]) == "/opt/dayz/@CF"
    assert _format_mod_paths([]) == ""
    print("test_format_mod_paths: PASSED")


def test_resolve_mod_source_local():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "@CF").mkdir()
        found = _resolve_mod_source("CF", local_mod_dirs=[root])
        assert found == root / "@CF"
    print("test_resolve_mod_source_local: PASSED")


def test_resolve_mod_source_workshop():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir)
        (workshop / "@CF").mkdir()
        found = _resolve_mod_source("@CF", workshop_dir=workshop)
        assert found == workshop / "@CF"
    print("test_resolve_mod_source_workshop: PASSED")


def test_resolve_mod_source_by_display_name():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir)
        item = workshop / "1559212036"
        item.mkdir(parents=True)
        (item / "mod.cpp").write_text('name = "Community Framework";\n')
        (item / "meta.cpp").write_text('name = "CF";\n')

        found = _resolve_mod_source("@Community Framework", workshop_dir=workshop)
        assert found == item

        found = _resolve_mod_source("Community Framework", workshop_dir=workshop)
        assert found == item

        found = _resolve_mod_source("@CF", workshop_dir=workshop)
        assert found == item

        found = _resolve_mod_source("CF", workshop_dir=workshop)
        assert found == item
    print("test_resolve_mod_source_by_display_name: PASSED")


def test_find_bikey_files():
    with tempfile.TemporaryDirectory() as tmpdir:
        mod = Path(tmpdir) / "@CF"
        keys = mod / "keys"
        keys.mkdir(parents=True)
        (keys / "cf.bikey").write_text("key")
        (keys / "readme.txt").write_text("readme")
        found = _find_bikey_files(mod)
        assert len(found) == 1
        assert found[0].name == "cf.bikey"
    print("test_find_bikey_files: PASSED")


def test_find_bikey_files_capital_and_singular():
    with tempfile.TemporaryDirectory() as tmpdir:
        mod = Path(tmpdir) / "@PristineRepair"
        (mod / "Keys").mkdir(parents=True)
        (mod / "Keys" / "author1.bikey").write_text("key1")
        (mod / "Key").mkdir(parents=True)
        (mod / "Key" / "author2.bikey").write_text("key2")
        found = _find_bikey_files(mod)
        assert len(found) == 2
        assert {p.name for p in found} == {"author1.bikey", "author2.bikey"}
    print("test_find_bikey_files_capital_and_singular: PASSED")


def test_create_mod_wrapper_normalizes_case():
    with tempfile.TemporaryDirectory() as tmpdir:
        source = Path(tmpdir) / "mod_source"
        source.mkdir()
        (source / "Addons").mkdir()
        (source / "Addons" / "mod.pbo").write_text("pbo")
        (source / "Key").mkdir()
        (source / "Key" / "mod.bikey").write_text("key")

        instance_root = Path(tmpdir) / "instance"
        instance_root.mkdir()
        wrapper = _create_mod_wrapper(instance_root, "@Mod", source)

        assert wrapper.is_dir()
        assert (wrapper / "addons").is_symlink()
        assert (wrapper / "addons" / "mod.pbo").exists()
        assert (wrapper / "keys").is_symlink()
        assert (wrapper / "Key").exists() is False
        assert (wrapper / "keys" / "mod.bikey").exists()
    print("test_create_mod_wrapper_normalizes_case: PASSED")


def test_deploy_instance_creates_structure():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        base = root / "base_server"
        base.mkdir()
        (base / "DayZServer_x64").write_text("binary")
        (base / "addons").mkdir()

        workshop = root / "workshop"
        workshop.mkdir()
        mod = workshop / "@CF"
        mod.mkdir()
        (mod / "keys").mkdir()
        (mod / "keys" / "cf.bikey").write_text("key")

        instance_dir = root / "instance1"
        result = deploy_instance(
            instance_name="Instance1",
            base_server_dir=base,
            instance_dir=instance_dir,
            mod_paths=["@CF"],
            workshop_dir=workshop,
            cfg_content="// cfg",
        )

        assert result.success
        assert (instance_dir / "serverDZ.cfg").read_text() == "// cfg"
        assert (instance_dir / "profiles").is_dir()
        assert (instance_dir / "mpmissions").is_dir()
        assert (instance_dir / "keys" / "cf.bikey").exists()
        assert (instance_dir / "@CF").is_dir()
        assert (instance_dir / "@CF" / "keys").is_symlink()
        # The server binary must be copied (not symlinked) with an
        # instance-unique name so Steam/BattlEye treat each instance as a
        # distinct process. The first available normal binary is renamed to
        # DayZServer_instance_<id> for a stable launch name.
        binary = instance_dir / "DayZServer_instance_1"
        assert binary.exists()
        assert not binary.is_symlink()
        assert binary.read_text() == "binary"
        assert not (instance_dir / "DayZServer_x64").exists()
    print("test_deploy_instance_creates_structure: PASSED")


def test_generate_start_script():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_dir = Path(tmpdir)
        script = generate_start_script(
            instance_dir=instance_dir,
            port=2302,
            max_players=60,
            mod_paths=["@CF", "@MasPuertas"],
        )
        content = script.read_text()
        assert "#!/usr/bin/env bash" in content
        assert "./DayZServer_instance_1" in content
        assert '-mod="@CF;@MasPuertas"' in content
        assert "-config=serverDZ.cfg" in content
        assert "-port=2302" in content
        assert "-maxPlayers=60" in content
        assert os.access(script, os.X_OK)
    print("test_generate_start_script: PASSED")


def test_generate_systemd_service():
    with tempfile.TemporaryDirectory() as tmpdir:
        instance_dir = Path(tmpdir)
        unit = generate_systemd_service(
            instance_name="Test Instance",
            instance_dir=instance_dir,
            output_dir=instance_dir,
        )
        content = unit.read_text()
        assert "[Unit]" in content
        assert "[Service]" in content
        assert f"WorkingDirectory={instance_dir}" in content
        assert "dayz-test-instance.service" in unit.name
    print("test_generate_systemd_service: PASSED")


def test_deploy_all_instances():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        base = root / "base_server"
        base.mkdir()
        (base / "DayZServer_x64").write_text("binary")

        workshop = root / "workshop"
        workshop.mkdir()
        (workshop / "@CF").mkdir()

        instances = [
            {
                "name": "Server1",
                "id": 1,
                "cfg_content": "// cfg1",
                "mod_paths": ["@CF"],
                "port": 2302,
                "max_players": 60,
                "profile": "profiles",
            },
            {
                "name": "Server2",
                "id": 2,
                "cfg_content": "// cfg2",
                "mod_paths": ["@CF"],
                "port": 2402,
                "max_players": 40,
                "profile": "profiles",
            },
        ]

        results = deploy_all_instances(
            base_server_dir=base,
            instances=instances,
            projects_root=root,
            workshop_dir=workshop,
            generate_scripts=True,
            generate_systemd=True,
            systemd_output_dir=root / "systemd",
        )

        assert len(results) == 2
        for name, result in results.items():
            assert result.success
            assert result.start_script_path and result.start_script_path.exists()
            assert result.systemd_path and result.systemd_path.exists()
            script = result.start_script_path.read_text()
            expected_exe = f"DayZServer_instance_{result.instance_dir.name.split('_')[-1]}"
            assert expected_exe in script
    print("test_deploy_all_instances: PASSED")


if __name__ == "__main__":
    import os

    test_format_mod_paths()
    test_resolve_mod_source_local()
    test_resolve_mod_source_workshop()
    test_resolve_mod_source_by_display_name()
    test_find_bikey_files()
    test_find_bikey_files_capital_and_singular()
    test_create_mod_wrapper_normalizes_case()
    test_deploy_instance_creates_structure()
    test_generate_start_script()
    test_generate_systemd_service()
    test_deploy_all_instances()
    print("\nAll deployment tests passed!")
