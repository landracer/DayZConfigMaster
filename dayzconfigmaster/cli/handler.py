# CLI handler for DayZConfigMaster
# 
# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Command Line Interface Handler.

This module provides the dzl command with 25+ subcommands matching dayz-labs functionality.
All code is governed by the SOVEREIGN INDIVIDUAL LICENSE.

Provides the `dzl` command with 25+ subcommands matching dayz-labs functionality.
"""

import argparse
import sys
import os
from pathlib import Path
from typing import List, Dict, Any, Optional

# Import update module
try:
    from ..update.manager import UpdateManager
except ImportError:
    pass

# Import economy module
try:
    from ..economy import CeCoreXml, TypesXml, EventsXml, GlobalsXml, EnvironmentXml, CeNum
except ImportError:
    # Fallback for direct execution
    pass


class CLIHandler:
    """
    Command line interface handler for DayZConfigMaster.
    
    Subcommand categories (25+ total):
    - server: start, stop, restart, status, logs (5)
    - build: prebuild, build, sign, cache (4)
    - config: get, set, list, validate (4)
    - workshop: download, update, search, auth (4)
    - instance: create, delete, enable, disable, list (5)
    - preset: save, load, list, delete (4)
    - workdrive: mount, umount, status (3)
    - utils: hash, validate, clean, info (4)
    """

    def __init__(self, projects_root: str = None):
        self.projects_root = Path(projects_root or ".")
        self._setup_argparse()

    def _setup_argparse(self):
        """Set up argument parser with all subcommands."""
        self.parser = argparse.ArgumentParser(
            prog='dzl',
            description='DayZ Config Master - Cross-platform server and mod management'
        )

        # Global options
        self.parser.add_argument(
            '-r', '--root',
            type=str,
            default=self.projects_root,
            help='Projects root directory (default: current)'
        )
        self.parser.add_argument(
            '-v', '--verbose',
            action='store_true',
            help='Enable verbose output'
        )

        # Create subparsers
        subparsers = self.parser.add_subparsers(dest='command', help='Commands')

        # ========== SERVER COMMANDS (5) ==========
        server_parser = subparsers.add_parser('server', help='Server management')
        server_subparsers = server_parser.add_subparsers(dest='action')

        # server start
        server_start = server_subparsers.add_parser('start', help='Start server')
        server_start.add_argument('-i', '--instance', default='default', help='Instance name')
        server_start.add_argument('-m', '--mode', choices=['debug', 'normal'], default='debug')

        # server stop
        server_stop = server_subparsers.add_parser('stop', help='Stop server')
        server_stop.add_argument('-i', '--instance', default='default')

        # server restart
        server_restart = server_subparsers.add_parser('restart', help='Restart server')
        server_restart.add_argument('-i', '--instance', default='default')
        server_restart.add_argument('-m', '--mode', choices=['debug', 'normal'])

        # server status
        server_status = server_subparsers.add_parser('status', help='Server status')
        server_status.add_argument('-i', '--instance', default='default')

        # server logs
        server_logs = server_subparsers.add_parser('logs', help='Show server logs')
        server_logs.add_argument('-n', '--lines', type=int, default=50)
        server_logs.add_argument('-i', '--instance', default='default')
        server_logs.add_argument('--tail', action='store_true', help='Follow logs')

        # ========== BUILD COMMANDS (4) ==========
        build_parser = subparsers.add_parser('build', help='Build management')
        build_subparsers = build_parser.add_subparsers(dest='action')

        # build prebuild
        build_prebuild = build_subparsers.add_parser('prebuild', help='Run preflight checks')
        build_prebuild.add_argument('-m', '--mod', required=True, help='Mod name')

        # build build
        build_build = build_subparsers.add_parser('build', help='Build mod')
        build_build.add_argument('-m', '--mod', required=True, help='Mod name')
        build_build.add_argument('--clean', action='store_true', help='Clean before build')
        build_build.add_argument('--binarize', action='store_true', default=True)

        # build sign
        build_sign = build_subparsers.add_parser('sign', help='Sign mod')
        build_sign.add_argument('-m', '--mod', required=True, help='Mod name')
        build_sign.add_argument('-k', '--key', help='Key path')

        # build cache
        build_cache = build_subparsers.add_parser('cache', help='Manage build cache')
        build_cache.add_argument('--clear', action='store_true', help='Clear cache')
        build_cache.add_argument('--info', action='store_true', help='Show cache info')

        # ========== CONFIG COMMANDS (4) ==========
        config_parser = subparsers.add_parser('config', help='Configuration management')
        config_subparsers = config_parser.add_subparsers(dest='action')

        # config get
        config_get = config_subparsers.add_parser('get', help='Get config value')
        config_get.add_argument('-k', '--key', required=True, help='Config key')
        config_get.add_argument('-i', '--instance', help='Instance name')

        # config set
        config_set = config_subparsers.add_parser('set', help='Set config value')
        config_set.add_argument('-k', '--key', required=True, help='Config key')
        config_set.add_argument('-v', '--value', required=True, help='Value')

        # config list
        config_list = config_subparsers.add_parser('list', help='List all configs')
        config_list.add_argument('-i', '--instance', help='Instance name')

        # config validate
        config_validate = config_subparsers.add_parser('validate', help='Validate configuration')
        config_validate.add_argument('-f', '--file', required=True, help='Config file path')

        # ========== WORKSHOP COMMANDS (4) ==========
        workshop_parser = subparsers.add_parser('workshop', help='Workshop management')
        workshop_subparsers = workshop_parser.add_subparsers(dest='action')

        # workshop download
        workshop_download = workshop_subparsers.add_parser('download', help='Download workshop item')
        workshop_download.add_argument('-i', '--id', required=True, type=int, help='Item ID')
        workshop_download.add_argument('--visible', action='store_true', help='Show console output')

        # workshop update
        workshop_update = workshop_subparsers.add_parser('update', help='Update workshop item')
        workshop_update.add_argument('-i', '--id', required=True, type=int, help='Item ID')

        # workshop search
        workshop_search = workshop_subparsers.add_parser('search', help='Search workshop')
        workshop_search.add_argument('query', help='Search query')

        # workshop auth
        workshop_auth = workshop_subparsers.add_parser('auth', help='Authentication status')
        workshop_auth.add_argument('--login', action='store_true', help='Check login status')

        # workshop upload
        workshop_upload = workshop_subparsers.add_parser('upload', help='Upload mod to Steam Workshop')
        workshop_upload.add_argument('-f', '--folder', required=True, help='Content folder path')
        workshop_upload.add_argument('-t', '--title', required=True, help='Workshop item title (max 128 chars)')
        workshop_upload.add_argument('-d', '--desc', default='', help='Description (max 8000 chars)')
        workshop_upload.add_argument('-p', '--preview', default='', help='Preview image path (< 1 MB)')
        workshop_upload.add_argument('--tags', default='', help='Comma-separated tags')
        workshop_upload.add_argument('--visibility', type=int, default=0,
                                     choices=[0, 1, 2], help='Visibility: 0=public, 1=friends, 2=private')
        workshop_upload.add_argument('--note', default='', help='Change note/changelog')
        workshop_upload.add_argument('-i', '--id', type=int, default=None, help='Existing item ID (for updates)')

        # ========== INSTANCE COMMANDS (5) ==========
        instance_parser = subparsers.add_parser('instance', help='Instance management')
        instance_subparsers = instance_parser.add_subparsers(dest='action')

        # instance create
        instance_create = instance_subparsers.add_parser('create', help='Create new instance')
        instance_create.add_argument('-n', '--name', required=True, help='Instance name')
        instance_create.add_argument('-p', '--port', type=int, default=2302)

        # instance delete
        instance_delete = instance_subparsers.add_parser('delete', help='Delete instance')
        instance_delete.add_argument('-n', '--name', required=True, help='Instance name')

        # instance enable
        instance_enable = instance_subparsers.add_parser('enable', help='Enable instance')
        instance_enable.add_argument('-n', '--name', required=True)

        # instance disable
        instance_disable = instance_subparsers.add_parser('disable', help='Disable instance')
        instance_disable.add_argument('-n', '--name', required=True)

        # instance list
        instance_list = instance_subparsers.add_parser('list', help='List all instances')

        # ========== PRESET COMMANDS (4) ==========
        preset_parser = subparsers.add_parser('preset', help='Preset management')
        preset_subparsers = preset_parser.add_subparsers(dest='action')

        # preset save
        preset_save = preset_subparsers.add_parser('save', help='Save current config as preset')
        preset_save.add_argument('-n', '--name', required=True, help='Preset name')

        # preset load
        preset_load = preset_subparsers.add_parser('load', help='Load preset')
        preset_load.add_argument('-n', '--name', required=True, help='Preset name')

        # preset list
        preset_list = preset_subparsers.add_parser('list', help='List presets')

        # preset delete
        preset_delete = preset_subparsers.add_parser('delete', help='Delete preset')
        preset_delete.add_argument('-n', '--name', required=True, help='Preset name')

        # ========== TOOLS COMMANDS (P2.1) ==========
        tools_parser = subparsers.add_parser('tools', help='DayZ Tools management')
        tools_subparsers = tools_parser.add_subparsers(dest='action')

        # tools list
        tools_list_cmd = tools_subparsers.add_parser('list', help='List available DayZ Tools')
        tools_list_cmd.add_argument('-p', '--path', default=None, help='Path to DayZ Tools root')

        # tools binarize
        tools_binarize = tools_subparsers.add_parser('binarize', help='Binarize models with binarize.exe')
        tools_binarize.add_argument('-s', '--source', required=True, help='Source directory')
        tools_binarize.add_argument('-d', '--dest', required=True, help='Destination directory')
        tools_binarize.add_argument('-p', '--path', required=True, help='DayZ Tools path')

        # tools convert
        tools_convert = tools_subparsers.add_parser('convert', help='Convert config files with CfgConvert.exe')
        tools_convert.add_argument('-i', '--input', required=True, help='Input file (.bin or .cpp)')
        tools_convert.add_argument('-o', '--output', required=True, help='Output file path')
        tools_convert.add_argument('-p', '--path', required=True, help='DayZ Tools path')

        # tools imagetopaa
        tools_paa = tools_subparsers.add_parser('imagetopaa', help='Convert images to PAA format')
        tools_paa.add_argument('-i', '--input', required=True, help='Input image file or directory')
        tools_paa.add_argument('-o', '--output', default=None, help='Output path (for single file)')
        tools_paa.add_argument('-p', '--path', required=True, help='DayZ Tools path')
        tools_paa.add_argument('--recursive', action='store_true', help='Process subdirectories')

        # ========== MOD PROJECT COMMANDS (P2.2) ==========
        mod_parser = subparsers.add_parser('mod', help='Mod project management')
        mod_subparsers = mod_parser.add_subparsers(dest='action')

        # mod new
        mod_new_cmd = mod_subparsers.add_parser('new', help='Create a new mod scaffold')
        mod_new_cmd.add_argument('-n', '--name', required=True, help='Mod name')
        mod_new_cmd.add_argument('-a', '--author', default='Unknown', help='Author name')

        # mod link
        mod_link = mod_subparsers.add_parser('link', help='Create/repair junction/symlink for P drive')
        mod_link.add_argument('-l', '--link', required=True, help='Link path (P:\\ or /mnt/p)')
        mod_link.add_argument('-t', '--target', required=True, help='Target directory')

        # mod preset
        mod_preset = mod_subparsers.add_parser('preset', help='Mod preset management')
        mod_preset_subparsers = mod_preset.add_subparsers(dest='action')

        # mod preset save
        mod_preset_save = mod_preset_subparsers.add_parser('save', help='Save current mods as preset')
        mod_preset_save.add_argument('-n', '--name', required=True, help='Preset name')
        mod_preset_save.add_argument('-m', '--mods', nargs='+', default=[], help='Mod paths to enable')

        # mod preset load
        mod_preset_load = mod_preset_subparsers.add_parser('load', help='Load a preset')
        mod_preset_load.add_argument('-n', '--name', required=True, help='Preset name')

        # mod preset list
        mod_preset_list = mod_preset_subparsers.add_parser('list', help='List presets')

        # mod preset delete
        mod_preset_delete_cmd = mod_preset_subparsers.add_parser('delete', help='Delete a preset')
        mod_preset_delete_cmd.add_argument('-n', '--name', required=True, help='Preset name')

        # ========== GITHUB COMMANDS (P2.3) ==========
        github_parser = subparsers.add_parser('github', help='GitHub repository management')
        github_subparsers = github_parser.add_subparsers(dest='action')

        # github publish
        github_publish = github_subparsers.add_parser('publish', help='Publish mod to GitHub')
        github_publish.add_argument('-n', '--mod', required=True, help='Mod name')
        github_publish.add_argument('-o', '--owner', required=True, help='GitHub owner/organization')
        github_publish.add_argument('-r', '--repo', required=True, help='Repository name')
        github_publish.add_argument('--private', action='store_true', help='Create private repo')
        github_publish.add_argument('-d', '--description', default=None, help='Repo description')

        # github release
        github_release = github_subparsers.add_parser('release', help='Create GitHub release')
        github_release.add_argument('-n', '--mod', required=True, help='Mod name')
        github_release.add_argument('-t', '--tag', required=True, help='Git tag (e.g., v1.0.0)')
        github_release.add_argument('--title', default=None, help='Release title')
        github_release.add_argument('--notes', default=None, help='Release notes')
        github_release.add_argument('--prerelease', action='store_true', help='Mark as pre-release')

        # ========== WORKDRIVE COMMANDS (3) ==========
        workdrive_parser = subparsers.add_parser('workdrive', help='Work drive management')
        workdrive_subparsers = workdrive_parser.add_subparsers(dest='action')

        # workdrive mount
        workdrive_mount = workdrive_subparsers.add_parser('mount', help='Mount work drive')
        workdrive_mount.add_argument('-p', '--path', required=True, help='Target path')

        # workdrive umount
        workdrive_umount = workdrive_subparsers.add_parser('umount', help='Unmount work drive')

        # workdrive status
        workdrive_status = workdrive_subparsers.add_parser('status', help='Show mount status')

        # ========== UTILS COMMANDS (4) ==========
        utils_parser = subparsers.add_parser('utils', help='Utility functions')
        utils_subparsers = utils_parser.add_subparsers(dest='action')

        # utils hash
        utils_hash = utils_subparsers.add_parser('hash', help='Calculate file hash')
        utils_hash.add_argument('-f', '--file', required=True, help='File path')

        # utils validate
        utils_validate = utils_subparsers.add_parser('validate', help='Validate mod files')
        utils_validate.add_argument('-m', '--mod', required=True, help='Mod name')

        # utils clean
        utils_clean = utils_subparsers.add_parser('clean', help='Clean build artifacts')
        utils_clean.add_argument('--all', action='store_true', help='Clean all')

        # utils info
        utils_info = utils_subparsers.add_parser('info', help='Show system info')
        utils_info.add_argument('--json', action='store_true', help='Output as JSON')

        # ========== PREFLIGHT COMMANDS (1) ==========
        preflight_parser = subparsers.add_parser('preflight', help='Run preflight checks')
        preflight_parser.add_argument('-m', '--mod', required=True, help='Mod name')

        # ========== ECONOMY COMMANDS (3) ==========
        economy_parser = subparsers.add_parser('economy', help='Central Economy management')
        economy_subparsers = economy_parser.add_subparsers(dest='action')

        # economy edit
        economy_edit = economy_subparsers.add_parser('edit', help='Edit Central Economy files')
        economy_edit.add_argument('-f', '--file', required=True, help='CE file to edit (types, globals, events, environment)')
        economy_edit.add_argument('-k', '--key', required=True, help='Setting key name')
        economy_edit.add_argument('-v', '--value', required=True, help='Value to set')

        # economy list
        economy_list = economy_subparsers.add_parser('list', help='List CE settings')
        economy_list.add_argument('-f', '--file', help='types.xml file path or folder path')
        economy_list.add_argument('-t', '--type', choices=['types', 'globals', 'events', 'environment'], help='Type of settings to list')

        # economy validate
        economy_validate = economy_subparsers.add_parser('validate', help='Validate Central Economy files')
        economy_validate.add_argument('-f', '--folder', required=True, help='CE folder path')

        # economy batch
        economy_batch = economy_subparsers.add_parser('batch', help='Batch-edit types.xml')
        economy_batch.add_argument('-f', '--file', required=True, help='types.xml file path or CE folder')
        economy_batch.add_argument('--op', required=True,
            choices=['scale-nominal','scale-min','scale-lifetime','scale-restock','set-category','add-usage','add-value'])
        economy_batch.add_argument('--value', required=True, help='Percent (for scale) or name (for set/add)')
        economy_batch.add_argument('--names', nargs='+', required=True, help='Type names to modify')

        # economy import-classnames
        economy_import = economy_subparsers.add_parser('import-classnames', help='Create types from a name list')
        economy_import.add_argument('-f', '--file', required=True, help='types.xml file path or CE folder')
        economy_import.add_argument('--text', required=True, help='Comma/space/newline-separated names')

        # economy backups
        economy_backups = economy_subparsers.add_parser('backups', help='List types.xml backups')
        economy_backups.add_argument('-f', '--file', required=True, help='types.xml file path or CE folder')

        # economy restore
        economy_restore = economy_subparsers.add_parser('restore', help='Restore a types.xml backup')
        economy_restore.add_argument('-f', '--file', required=True, help='Live types.xml file path')
        economy_restore.add_argument('-b', '--backup', required=True, help='Backup file to restore')

        # ========== LOGS COMMANDS (2) ==========
        logs_parser = subparsers.add_parser('logs', help='Log management')
        logs_subparsers = logs_parser.add_subparsers(dest='action')

        # logs tail
        logs_tail = logs_subparsers.add_parser('tail', help='Tail server logs')
        logs_tail.add_argument('-i', '--instance', default='default')
        logs_tail.add_argument('-n', '--lines', type=int, default=50)

        # logs show
        logs_show = logs_subparsers.add_parser('show', help='Show log file')
        logs_show.add_argument('-f', '--file', required=True, help='Log file path')

        # ========== UPDATE COMMANDS (3) ==========
        update_parser = subparsers.add_parser('update', help='Update management')
        update_subparsers = update_parser.add_subparsers(dest='action')

        # update check
        update_check = update_subparsers.add_parser('check', help='Check for updates')
        update_check.add_argument('--json', action='store_true', help='Output as JSON')

        # update download
        update_download = update_subparsers.add_parser('download', help='Download latest update')

        # update install
        update_install = update_subparsers.add_parser('install', help='Install downloaded update')

    def parse_args(self, args: List[str] = None) -> argparse.Namespace:
        """Parse command line arguments."""
        return self.parser.parse_args(args)

    def run(self, args: List[str] = None) -> int:
        """
        Run the CLI with given arguments.

        Args:
            args: Command line arguments (defaults to sys.argv[1:])

        Returns:
            Exit code (0 for success, non-zero for error)
        """
        parsed = self.parse_args(args)

        if not parsed.command:
            self.parser.print_help()
            return 1

        # Route to appropriate handler
        handler_map = {
            'server': self._handle_server,
            'build': self._handle_build,
            'config': self._handle_config,
            'workshop': self._handle_workshop,
            'instance': self._handle_instance,
            'preset': self._handle_preset,
            'tools': self._handle_tools,
            'mod': self._handle_mod,
            'github': self._handle_github,
            'workdrive': self._handle_workdrive,
            'utils': self._handle_utils,
            'preflight': self._handle_preflight,
            'logs': self._handle_logs,
            'economy': self._handle_economy,
            'update': self._handle_update,
        }

        handler = handler_map.get(parsed.command)
        if handler:
            try:
                result = handler(parsed)
                return 0 if result else 1
            except Exception as e:
                print(f"Error: {e}", file=sys.stderr)
                return 1

        return 1

    def _handle_server(self, args: argparse.Namespace) -> bool:
        """Handle server subcommands."""
        from dayzconfigmaster.server.instance_manager import InstanceManager

        manager = InstanceManager(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'start':
            instance = manager.get_instance(args.instance)
            if not instance:
                print(f"Instance '{args.instance}' not found")
                return False
            print(f"Starting server: {instance.name}")
            # Would start the server here
            return True

        elif action == 'stop':
            print(f"Stopping server: {args.instance}")
            return True

        elif action == 'restart':
            print(f"Restarting server: {args.instance}")
            return True

        elif action == 'status':
            status = {"running": False, "mode": "unknown"}
            print(f"Server status: {status}")
            return True

        elif action == 'logs':
            lines = args.lines
            print(f"Showing last {lines} log lines")
            return True

        return False

    def _handle_build(self, args: argparse.Namespace) -> bool:
        """Handle build subcommands."""
        from dayzconfigmaster.build.service import BuildService

        service = BuildService(
            str(self.projects_root),
            str(self.projects_root / "tools")
        )

        action = getattr(args, 'action', '')

        if action == 'prebuild':
            result = service.preflight(args.mod)
            self._print_result(result)
            return result.get("ok", True)

        elif action == 'build':
            result = service.build(
                args.mod,
                clean=args.clean
            )
            self._print_result(result)
            return result.get("success", False)

        elif action == 'sign':
            success, message = service.generate_key()
            print(f"Sign: {message}")
            return success

        elif action == 'cache':
            if args.clear:
                from dayzconfigmaster.build.cache import BuildCache
                cache = BuildCache(str(self.projects_root / ".build_cache"))
                count = cache.invalidate_all()
                print(f"Cleared {count} cache entries")
                return True

            if args.info:
                from dayzconfigmaster.build.cache import BuildCache
                cache = BuildCache(str(self.projects_root / ".build_cache"))
                size = cache.get_size()
                print(f"Build cache: {size} entries")
                return True

        return False

    def _handle_config(self, args: argparse.Namespace) -> bool:
        """Handle config subcommands."""
        from dayzconfigmaster.config.dzl_config import DzlConfig

        config = DzlConfig(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'get':
            instance = args.instance
            value = config.resolve_active(instance).get(args.key)
            print(f"{args.key} = {value}")
            return True

        elif action == 'set':
            config.set(args.key, args.value)
            print(f"Set {args.key} = {args.value}")
            return True

        elif action == 'list':
            resolved = config.resolve_active()
            for key, value in resolved.items():
                print(f"{key}: {value}")
            return True

        elif action == 'validate':
            # Validate configuration file
            print(f"Validating: {args.file}")
            return True

        return False

    def _handle_workshop(self, args: argparse.Namespace) -> bool:
        """Handle workshop subcommands with robust API integration."""
        from dayzconfigmaster.workshop.api import WorkshopManager, SteamWorkshopAPI

        # Initialize workshop manager
        workshop_manager = WorkshopManager(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'download':
            result = workshop_manager.download_item(str(args.id))
            print(f"Download: {result[1]}")
            return result[0]

        elif action == 'update':
            result = workshop_manager.download_item(str(args.id))
            print(f"Update: {result[1]}")
            return result[0]

        elif action == 'search':
            items = []
            success, results = workshop_manager.search(args.query)

            if results:
                for item in results[:20]:  # Limit to 20 results
                    print(f"  {item.id}: {item.title}")
                    items.append(item)

                print(f"\nFound {len(items)} items")
                return True

            print("No workshop items found")
            return False

        elif action == 'auth':
            # Check for Steam API key configuration
            api = SteamWorkshopAPI()
            if api.api_key:
                print("Authentication status: API key configured")
            else:
                print("Authentication status: anonymous (no API key)")
                print("Tip: Set STEAM_API_KEY environment variable or pass --api-key")
            return True

        elif action == 'upload':
            # Import uploader and create request
            try:
                from dayzconfigmaster.workshop.uploader import WorkshopUploader, WorkshopUploadRequest

                tags = []
                if args.tags:
                    tags = [t.strip() for t in args.tags.split(',') if t.strip()]

                req = WorkshopUploadRequest(
                    content_folder=args.folder,
                    title=args.title,
                    description=args.desc,
                    preview_file=args.preview,
                    tags=tags,
                    visibility=args.visibility,
                    change_note=args.note,
                    published_file_id=args.id
                )

                uploader = WorkshopUploader()
                success, message, file_id = uploader.publish(req)

                if success:
                    print(f"Workshop upload successful!")
                    print(f"Item ID: {file_id}")
                    print(f"Message: {message}")
                else:
                    print(f"Upload failed: {message}")

                return success
            except Exception as e:
                print(f"Upload error: {e}")
                import traceback
                traceback.print_exc()
                return False

        elif action == 'info':
            # Get detailed info about a workshop item
            api = SteamWorkshopAPI()
            if hasattr(args, 'item_id') and args.item_id:
                info = api.get_item_details(str(args.item_id))
                if info:
                    print(f"Title: {info.title}")
                    print(f"ID: {info.id}")
                    print(f"Subscribers: {info.subscribers:,}")
                    print(f"Rating: {'*' * int(info.rating)} ({info.rating:.1f}/5)")
                    print(f"Tags: {', '.join(info.tags[:5]) if info.tags else 'none'}")
                    return True
                else:
                    print("Item not found")
            else:
                print("Specify --item-id to get item details")
            return False

        return False

    def _handle_instance(self, args: argparse.Namespace) -> bool:
        """Handle instance subcommands."""
        from dayzconfigmaster.server.instance_manager import InstanceManager

        manager = InstanceManager(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'create':
            success, message = manager.create_instance(
                args.name,
                port=args.port
            )
            print(message)
            return success

        elif action == 'delete':
            success, message = manager.delete_instance(args.name)
            print(message)
            return success

        elif action == 'enable':
            instance = manager.get_instance(args.name)
            if instance:
                instance.enabled = True
                manager.save_instances()
                print(f"Enabled: {args.name}")
                return True
            return False

        elif action == 'disable':
            instance = manager.get_instance(args.name)
            if instance:
                instance.enabled = False
                manager.save_instances()
                print(f"Disabled: {args.name}")
                return True
            return False

        elif action == 'list':
            instances = manager.list_instances()
            for inst in instances:
                status = "enabled" if inst.enabled else "disabled"
                print(f"{inst.name}: {status} (port: {inst.port})")
            return True

        return False

    def _handle_preset(self, args: argparse.Namespace) -> bool:
        """Handle preset subcommands."""
        from dayzconfigmaster.server.profiles import ProfileManager

        manager = ProfileManager(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'save':
            success, message = manager.save_profile(
                args.name,
                mod_list=[],
                server_config={}
            )
            print(message)
            return success

        elif action == 'load':
            profile = manager.load_profile(args.name)
            if profile:
                print(f"Loaded preset: {args.name}")
                return True
            return False

        elif action == 'list':
            presets = manager.list_profiles()
            for preset in presets:
                print(f"- {preset}")
            return True

        elif action == 'delete':
            success, message = manager.delete_profile(args.name)
            print(message)
            return success

        return False

    def _handle_tools(self, args: argparse.Namespace) -> bool:
        """Handle DayZ Tools subcommands."""
        action = getattr(args, 'action', '')

        if action == 'list':
            from dayzconfigmaster.tools import find_binarize, find_cfgconvert, find_imagetopaa
            tools_path = args.path or os.environ.get('DAYZ_TOOLS_PATH')
            if not tools_path:
                print("Error: Please specify DayZ Tools path with -p/--path")
                return False

            binarize_exists = find_binarize(tools_path) is not None
            cfgconvert_exists = find_cfgconvert(tools_path) is not None
            imagetopaa_exists = find_imagetopaa(tools_path) is not None

            print("DayZ Tools status:")
            print(f"  binarize: {'installed' if binarize_exists else 'MISSING'}")
            print(f"  CfgConvert: {'installed' if cfgconvert_exists else 'MISSING'}")
            print(f"  ImageToPAA: {'installed' if imagetopaa_exists else 'MISSING'}")

            return True

        elif action == 'binarize':
            from dayzconfigmaster.tools import binarize_folder
            success, message = binarize_folder(
                source_dir=args.source,
                dest_dir=args.dest,
                tools_path=args.path
            )
            print(f"Binarize: {message}")
            return success

        elif action == 'convert':
            from dayzconfigmaster.tools import convert_to_cpp, convert_to_bin
            input_path = Path(args.input)

            if input_path.suffix.lower() == '.bin':
                success, message = convert_to_cpp(args.input, args.output, args.path)
            else:
                success, message = convert_to_bin(args.input, args.output, args.path)

            print(f"Convert: {message}")
            return success

        elif action == 'imagetopaa':
            from dayzconfigmaster.tools import convert_folder
            recursive = getattr(args, 'recursive', False)
            results = convert_folder(args.input, args.path, recursive)

            for input_path, output_path, success, message in results:
                status = "OK" if success else "FAILED"
                print(f"[{status}] {input_path} -> {output_path}")
                if not success:
                    print(f"  Error: {message}")

            all_success = all(r[2] for r in results)
            return all_success

        return False

    def _handle_mod(self, args: argparse.Namespace) -> bool:
        """Handle mod project subcommands."""
        from dayzconfigmaster.mods.scaffold import (
            scaffold_mod,
            fix_junction,
            save_mod_preset,
            load_mod_preset,
            list_mod_presets,
            delete_mod_preset
        )

        action = getattr(args, 'action', '')

        if action == 'new':
            success, message = scaffold_mod(
                str(self.projects_root),
                args.name,
                args.author
            )
            print(f"Mod scaffold: {message}")
            return success

        elif action == 'link':
            success, message = fix_junction(args.link, args.target)
            print(f"Junction: {message}")
            return success

        elif action == 'preset':
            # Check if we have nested preset actions
            preset_action = getattr(args, 'action', '')
            # Use mod_preset_action for the subparser's actual action
            if hasattr(args, '__dict__'):
                # Get any subparser action from args
                for attr in ['save', 'load', 'list', 'delete']:
                    if getattr(args, attr, None) is not None:
                        preset_action = attr
                        break

            if preset_action == 'save':
                mods = getattr(args, 'mods', []) or []
                success, message = save_mod_preset(
                    str(self.projects_root),
                    args.name,
                    mods
                )
                print(f"Mod preset: {message}")
                return success

            elif preset_action == 'load':
                success, data = load_mod_preset(str(self.projects_root), args.name)
                if success:
                    print(f"Loaded preset '{args.name}':")
                    for mod in data.get('mods', []):
                        print(f"  - {mod}")
                    return True
                else:
                    print(f"Preset not found: {args.name}")
                    return False

            elif preset_action == 'list':
                presets = list_mod_presets(str(self.projects_root))
                if presets:
                    print("Available presets:")
                    for p in presets:
                        print(f"  - {p}")
                    return True
                else:
                    print("No presets found")
                    return True

            elif preset_action == 'delete':
                success, message = delete_mod_preset(str(self.projects_root), args.name)
                print(f"Preset: {message}")
                return success

        return False

    def _handle_github(self, args: argparse.Namespace) -> bool:
        """Handle GitHub repository subcommands."""
        from dayzconfigmaster.git.manager import GitManager

        action = getattr(args, 'action', '')

        manager = GitManager(str(self.projects_root))

        if action == 'publish':
            success, message = manager.publish_repo(
                mod_name=args.mod,
                github_owner=args.owner,
                repo_name=args.repo,
                private=getattr(args, 'private', False),
                description=args.description
            )
            print(f"GitHub publish: {message}")
            return success

        elif action == 'release':
            success, message = manager.create_release(
                mod_name=args.mod,
                tag=args.tag,
                title=args.title,
                notes=args.notes,
                prerelease=getattr(args, 'prerelease', False)
            )
            print(f"GitHub release: {message}")
            return success

        return False

    def _handle_workdrive(self, args: argparse.Namespace) -> bool:
        """Handle work drive subcommands."""
        from dayzconfigmaster.workdrive import WorkDrive

        workdrive = WorkDrive(str(self.projects_root))

        action = getattr(args, 'action', '')

        if action == 'mount':
            result = workdrive.mount_work_drive(args.path)
            print(result.message)
            return result.success

        elif action == 'umount':
            success = workdrive.unmount_work_drive()
            print("Unmounted" if success else "Failed to unmount")
            return success

        elif action == 'status':
            status = workdrive.get_status()
            print(f"Work drive status: {status}")
            return True

        return False

    def _handle_utils(self, args: argparse.Namespace) -> bool:
        """Handle utility subcommands."""
        import hashlib
        from pathlib import Path

        action = getattr(args, 'action', '')

        if action == 'hash':
            file_path = Path(args.file)
            sha256 = hashlib.sha256()

            with open(file_path, 'rb') as f:
                for chunk in iter(lambda: f.read(8192), b''):
                    sha256.update(chunk)

            print(f"SHA256: {sha256.hexdigest()}")
            return True

        elif action == 'validate':
            from dayzconfigmaster.build.preflight import PreflightEngine
            engine = PreflightEngine()

            mod_dir = self.projects_root / "mods" / args.mod
            result = engine.run(str(mod_dir))
            self._print_result(result)
            return result.get("ok", True)

        elif action == 'clean':
            if args.all:
                import shutil
                build_dir = self.projects_root / "build"
                if build_dir.exists():
                    shutil.rmtree(build_dir)
                print("Cleaned all builds")
            return True

        elif action == 'info':
            import platform
            info = {
                "platform": platform.system(),
                "python_version": platform.python_version(),
                "projects_root": str(self.projects_root),
                "executable": sys.executable
            }

            if args.json:
                import json
                print(json.dumps(info, indent=2))
            else:
                for key, value in info.items():
                    print(f"{key}: {value}")
            return True

        return False

    def _handle_preflight(self, args: argparse.Namespace) -> bool:
        """Handle preflight checks."""
        from dayzconfigmaster.build.preflight import PreflightEngine

        mod_dir = self.projects_root / "mods" / args.mod
        engine = PreflightEngine()

        result = engine.run(str(mod_dir))
        self._print_result(result)

        return result.get("ok", True)

    def _resolve_ce_file(self, arg) -> Optional[str]:
        """Resolve a CE file argument to a real path. Returns None if not found."""
        from pathlib import Path
        
        if not arg:
            return None
            
        # If it's an existing file, use it directly
        p = Path(arg)
        if p.is_file():
            return str(p)
        
        # If it's a directory, look for types.xml
        if p.is_dir():
            types_path = p / "types.xml"
            if types_path.exists():
                return str(types_path)
            # Also check .xml extension on directory name
            xml_path = Path(str(arg) + ".xml")
            if xml_path.exists():
                return str(xml_path)
        
        # Try the arg as a path with potential .xml extension
        p_with_xml = Path(str(arg) + ".xml")
        if p_with_xml.is_file():
            return str(p_with_xml)
            
        return None

    def _handle_economy(self, args: argparse.Namespace) -> bool:
        """Handle economy subcommands."""
        from dayzconfigmaster.economy import TypesXml
        from pathlib import Path

        action = getattr(args, 'action', '')

        if action == 'edit':
            path = self._resolve_ce_file(args.file)
            if not path or not Path(path).exists():
                print(f"File not found: {args.file}")
                return False
            
            types = TypesXml.from_file(path)
            if types is None:
                print(f"Failed to parse: {path}")
                return False
            
            # Parse key=value format from args.key
            # Format: "Apple.nominal" or just "nominal=99"
            key = getattr(args, 'key', '')
            value = getattr(args, 'value', '')
            
            if '.' in key:
                # Format like "Apple.nominal"
                type_name, field = key.split('.', 1)
                entry = types.get_type(type_name)
                if not entry:
                    print(f"Type not found: {type_name}")
                    return False
                setattr(entry, field, int(value) if value.isdigit() else value)
            elif '=' in key:
                # Format like "nominal=99"
                field, val = key.split('=', 1)
                # Look for types using args.key as type name if no dot
                type_name = getattr(args, 'key', '')
                entry = types.get_type(type_name)
                if not entry:
                    print(f"Type not found: {type_name}")
                    return False
                setattr(entry, field, int(val) if val.isdigit() else val)
            else:
                # Try to find the type from args.file context or use first type
                all_types = types.get_all_types()
                if all_types:
                    entry = list(all_types.values())[0]
                else:
                    print("No types found in file")
                    return False
                setattr(entry, key, int(value) if value.isdigit() else value)
            
            # Backup before saving
            backup_path = types.backup_types()
            Path(path).write_text(types.to_xml(), encoding='utf-8')
            print(f"Saved {path}")
            if backup_path:
                print(f"Backup: {backup_path}")
            return True

        elif action == 'list':
            path = self._resolve_ce_file(getattr(args, 'file', None) or getattr(args, 'type', None))
            if not path or not Path(path).exists():
                print(f"File not found")
                return False
            
            types = TypesXml.from_file(path)
            if types is None:
                print(f"Failed to parse: {path}")
                return False
            
            for name, entry in types.get_all_types().items():
                print(f"{entry.name}: nominal={entry.nominal} min={entry.min} "
                      f"lifetime={entry.lifetime} restock={entry.restock}")
            return True

        elif action == 'validate':
            folder = Path(args.folder)
            if not folder.exists():
                print(f"Folder not found: {args.folder}")
                return False
            
            types_file = folder / "types.xml"
            if types_file.exists():
                types = TypesXml.from_file(str(types_file))
                if types:
                    count = len(types.get_all_types())
                    print(f"types.xml: OK ({count} types)")
                else:
                    print("types.xml: PARSE ERROR")
            else:
                print("types.xml not found")
            return True

        elif action == 'batch':
            path = self._resolve_ce_file(args.file)
            if not path or not Path(path).exists():
                print(f"File not found: {args.file}")
                return False
            
            types = TypesXml.from_file(path)
            if types is None:
                print(f"Failed to parse: {path}")
                return False
            
            op = getattr(args, 'op', '')
            names = args.names
            value = getattr(args, 'value', '')
            
            if op.startswith('scale-'):
                field = op.split('-', 1)[1]
                types.batch_scale(names, field, float(value))
            elif op == 'set-category':
                types.batch_set_category(names, value)
            elif op in ('add-usage', 'add-value'):
                types.batch_add_flag(names, op.split('-')[1], value)
            
            # Backup and save
            backup_path = types.backup_types()
            Path(path).write_text(types.to_xml(), encoding='utf-8')
            print(f"Batch {op} applied to {len(names)} types; saved {path}")
            if backup_path:
                print(f"Backup: {backup_path}")
            return True

        elif action == 'import-classnames':
            path = self._resolve_ce_file(args.file)
            if not path or not Path(path).exists():
                print(f"File not found: {args.file}")
                return False
            
            types = TypesXml.from_file(path)
            if types is None:
                print(f"Failed to parse: {path}")
                return False
            
            text = getattr(args, 'text', '')
            added = types.import_classnames(text)
            
            backup_path = types.backup_types()
            Path(path).write_text(types.to_xml(), encoding='utf-8')
            print(f"Imported {len(added)} new types; saved {path}")
            if backup_path:
                print(f"Backup: {backup_path}")
            return True

        elif action == 'backups':
            path = self._resolve_ce_file(args.file)
            if not path or not Path(path).exists():
                print(f"File not found: {args.file}")
                return False
            
            types = TypesXml.from_file(path)
            backups = types.list_backups()
            
            if not backups:
                print("No backups found")
                return True
            
            print("Available backups:")
            for b in backups[:20]:  # Show newest 20
                print(f"  {b}")
            return True

        elif action == 'restore':
            path = self._resolve_ce_file(args.file)
            if not path or not Path(path).exists():
                print(f"File not found: {args.file}")
                return False
            
            backup_path = getattr(args, 'backup', None)
            if not backup_path:
                print("Backup file required: use -b/--backup")
                return False
            
            types = TypesXml.from_file(path)
            if types is None:
                print(f"Failed to parse: {path}")
                return False
            
            from pathlib import Path as PathClass
            if types.restore_backup(PathClass(backup_path)):
                print(f"Restored from: {backup_path}")
                # Save the restored state
                Path(path).write_text(types.to_xml(), encoding='utf-8')
                return True
            else:
                print("Restore failed")
                return False

        return False

    def _handle_logs(self, args: argparse.Namespace) -> bool:
        """Handle log subcommands."""
        from dayzconfigmaster.logs.tailer import LogTailer
        
        action = getattr(args, 'action', '')
        
        if action == 'tail':
            profile_path = self.projects_root / "profiles"
            tailer = LogTailer(str(profile_path))
            
            lines = args.lines
            log_type = getattr(args, 'type', 'script')  # Default to script logs
            
            result = tailer.tail(log_type, lines)
            
            if result and len(result) > 0:
                for line in result:
                    print(line)
            else:
                print("No logs found")
            return True

        elif action == 'show':
            log_path = getattr(args, 'file', None)
            if not log_path:
                print(f"Log file path required: use -f/--file")
                return False
            
            p = Path(log_path)
            if not p.exists():
                print(f"Log file not found: {log_path}")
                return False
            
            try:
                with open(p, 'r', encoding='utf-8', errors='ignore') as f:
                    content = f.read()
                
                # Show last N lines (default 50)
                lines = getattr(args, 'lines', 50)
                all_lines = content.split('\n')
                for line in all_lines[-lines:]:
                    print(line)
            except Exception as e:
                print(f"Error reading log: {e}")
                return False
            
            return True

        return False

    def _handle_update(self, args: argparse.Namespace) -> bool:
        """Handle update subcommands."""
        try:
            manager = UpdateManager()

            action = getattr(args, 'action', '')

            if action == 'check':
                # Get current version
                try:
                    from .. import __version__
                    current_version = __version__
                except ImportError:
                    current_version = "0.0.0"

                is_available, info = manager.check_for_updates(current_version)

                if args.json:
                    import json
                    result = {
                        "current_version": current_version,
                        "update_available": is_available
                    }
                    if is_available and info:
                        result["latest"] = {
                            "version": info.version,
                            "release_name": info.release_name,
                            "url": info.release_url
                        }
                    print(json.dumps(result, indent=2))
                else:
                    if is_available and info:
                        print(f"Update available: {info.version}")
                        print(f"Release: {info.release_name}")
                        print(f"URL: {info.release_url}")
                    else:
                        print(f"You are up to date (version {current_version})")
                return True

            elif action == 'download':
                try:
                    from .. import __version__
                    current_version = __version__
                except ImportError:
                    current_version = "0.0.0"

                is_available, info = manager.check_for_updates(current_version)

                if not is_available or not info:
                    print("No update available")
                    return False

                output_path = Path.home() / f"DZLUpdate_{info.version}.exe"
                success = manager.download_asset(info.assets[0]["browser_download_url"], output_path)

                if success:
                    print(f"Downloaded to: {output_path}")
                else:
                    print("Failed to download update")

                return success

            elif action == 'install':
                print("Install command - requires user interaction")
                # Install would be handled by user running the downloaded installer
                return True

        except ImportError:
            print("Update module not available")

        return False

    def _print_result(self, result: dict):
        """Print a result dictionary."""
        if not isinstance(result, dict):
            print(str(result))
            return

        for key, value in result.items():
            print(f"{key}: {value}")


def main() -> int:
    """
    Main entry point for CLI.

    Returns:
        Exit code
    """
    # Hard safety ceiling for the CLI process: it must never be allowed to
    # consume enough RAM to hard-lock the host.
    try:
        from dayzconfigmaster.utils.memory_guard import setup_memory_safety
        setup_memory_safety()
    except Exception as exc:
        print(f"Warning: could not start memory safety guard: {exc}", file=sys.stderr)

    # Try to get projects root from environment or use current directory
    projects_root = os.environ.get('DZL_PROJECTS_ROOT', '.')

    handler = CLIHandler(projects_root)
    return handler.run()


if __name__ == '__main__':
    import os

    sys.exit(main())
