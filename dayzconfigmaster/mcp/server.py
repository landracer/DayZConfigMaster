# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# MCP Server - JSON-RPC interface for AI agent automation

"""
MCP Server - Model Context Protocol server for AI agent integration.

Provides JSON-RPC endpoints for:
- Config operations (read/write/validate)
- Build operations (prebuild, build, sign)
- Process management (start, stop, status)
- Workshop downloads
- File system operations

Reference: https://modelcontextprotocol.io/specification
"""

import json
import logging
import os
import asyncio
import tempfile
import threading
import socket
from pathlib import Path
from typing import Dict, Any, Optional, List
from abc import ABC, abstractmethod

logger = logging.getLogger(__name__)

# Named pipe support for Windows
try:
    import win32pipe
    import win32file
    import pywintypes
    HAS_WIN32 = True
except ImportError:
    try:
        import ctypes
        from ctypes import wintypes
        HAS_WIN32 = True
    except ImportError:
        HAS_WIN32 = False

# Unix domain socket support for Linux/macOS
try:
    import socket as unix_socket
    HAS_UNIX_SOCKETS = True
except ImportError:
    HAS_UNIX_SOCKETS = False


class MCPMethod(ABC):
    """Base class for MCP methods."""
    
    @abstractmethod
    def name(self) -> str:
        """Return method name."""
        pass
    
    @abstractmethod
    def params_schema(self) -> dict:
        """Return parameter schema."""
        pass
    
    @abstractmethod
    async def execute(self, params: dict) -> Any:
        """Execute the method with given parameters."""
        pass


class MCPConfigMethod(MCPMethod):
    """Configuration-related methods."""
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
    
    def name(self) -> str:
        return "config"
    
    def params_schema(self) -> dict:
        return {
            "type": "object",
            "properties": {
                "action": {"type": "string", "enum": ["get", "set", "list"]},
                "key": {"type": "string"},
                "value": {},
                "instance": {"type": "string"}
            },
            "required": ["action"]
        }
    
    async def execute(self, params: dict) -> Any:
        from dayzconfigmaster.config.dzl_config import DzlConfig
        
        action = params.get("action")
        
        if action == "get":
            key = params.get("key", "")
            instance = params.get("instance")
            
            config = DzlConfig(str(self.projects_root))
            resolved = config.resolve_active(instance)
            
            if key:
                return {"value": resolved.get(key)}
            return {"value": resolved}
        
        elif action == "set":
            key = params.get("key", "")
            value = params.get("value")
            
            config = DzlConfig(str(self.projects_root))
            config.set(key, value)
            
            return {"success": True, "message": f"Set {key}"}
        
        elif action == "list":
            config = DzlConfig(str(self.projects_root))
            instances = config.list_instances()
            
            return {"instances": instances}
        
        return {"error": "Unknown action"}


class MCPBuildMethod(MCPMethod):
    """Build-related methods."""
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
    
    def name(self) -> str:
        return "build"
    
    def params_schema(self) -> dict:
        return {
            "type": "object",
            "properties": {
                "action": {"type": "string", "enum": ["prebuild", "build", "sign"]},
                "mod_name": {"type": "string"},
                "clean": {"type": "boolean"},
                "binarize": {"type": "boolean"}
            },
            "required": ["action"]
        }
    
    async def execute(self, params: dict) -> Any:
        from dayzconfigmaster.build.service import BuildService
        
        action = params.get("action")
        
        if action == "prebuild":
            mod_name = params.get("mod_name", "")
            
            preflight_engine = __import__('dayzconfigmaster.build.preflight', fromlist=['PreflightEngine']).PreflightEngine()
            report = preflight_engine.run(str(self.projects_root / "mods" / mod_name))
            
            return {"preflight": report}
        
        elif action == "build":
            mod_name = params.get("mod_name", "")
            
            service = BuildService(
                str(self.projects_root),
                str(self.projects_root / "tools")
            )
            
            result = service.build(mod_name)
            return result
        
        elif action == "sign":
            key_path = params.get("key_path", "")
            mod_name = params.get("mod_name", "")
            
            service = BuildService(
                str(self.projects_root),
                str(self.projects_root / "tools"),
                key_path
            )
            
            success, message = service.generate_key()
            return {"success": success, "message": message}
        
        return {"error": "Unknown action"}


class MCPProcessMethod(MCPMethod):
    """Process management methods."""
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
    
    def name(self) -> str:
        return "process"
    
    def params_schema(self) -> dict:
        return {
            "type": "object",
            "properties": {
                "action": {"type": "string", "enum": ["start", "stop", "status", "restart"]},
                "instance": {"type": "string"},
                "mode": {"type": "string", "enum": ["debug", "normal"]}
            },
            "required": ["action"]
        }
    
    async def execute(self, params: dict) -> Any:
        from dayzconfigmaster.server.process_controller import ProcessController
        
        action = params.get("action")
        
        if action == "start":
            instance = params.get("instance", "default")
            mode = params.get("mode", "debug")
            
            # Get config
            from dayzconfigmaster.config.dzl_config import DzlConfig
            config = DzlConfig(str(self.projects_root))
            resolved = config.resolve_active(instance)
            
            controller = ProcessController(resolved.get("dayz_path", ""))
            
            success, message = controller.start_server(
                port=resolved.get("port", 2302),
                mode=mode,
                max_players=resolved.get("max_players", 60),
                map_size=resolved.get("map_size", 2000),
                mods=[f'@{mod}' for mod in resolved.get("mods", [])]
            )
            
            return {"success": success, "message": message}
        
        elif action == "stop":
            controller = ProcessController("")
            controller.stop_server()
            return {"success": True, "message": "Process stopped"}
        
        elif action == "status":
            controller = ProcessController("")
            status = controller.get_status()
            return status
        
        elif action == "restart":
            instance = params.get("instance", "default")
            
            from dayzconfigmaster.config.dzl_config import DzlConfig
            config = DzlConfig(str(self.projects_root))
            resolved = config.resolve_active(instance)
            
            controller = ProcessController(resolved.get("dayz_path", ""))
            
            success, message = controller.restart_server(
                mode=resolved.get("mode", "debug"),
                port=resolved.get("port", 2302)
            )
            
            return {"success": success, "message": message}
        
        return {"error": "Unknown action"}


class MCPWorkshopMethod(MCPMethod):
    """Workshop-related methods."""
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
    
    def name(self) -> str:
        return "workshop"
    
    def params_schema(self) -> dict:
        return {
            "type": "object",
            "properties": {
                "action": {"type": "string", "enum": ["download", "update", "upload"]},
                "item_id": {"type": "integer"},
                "install_dir": {"type": "string"},
                "visible_console": {"type": "boolean"},
                "content_folder": {"type": "string"},
                "title": {"type": "string"},
                "description": {"type": "string"},
                "preview_file": {"type": "string"},
                "tags": {"type": "array", "items": {"type": "string"}},
                "visibility": {"type": "integer"},
                "change_note": {"type": "string"},
                "published_file_id": {"type": "integer"}
            },
            "required": ["action"]
        }
    
    async def execute(self, params: dict) -> Any:
        action = params.get("action")
        
        if action == "download":
            from dayzconfigmaster.workshop.steamcmd import ConsoleVisibleSteamCmd
            item_id = str(params.get("item_id", ""))
            install_dir = params.get("install_dir")
            
            steamcmd_path = self.projects_root / "steamcmd"
            steamcmd = ConsoleVisibleSteamCmd(str(steamcmd_path))
            
            visible_console = params.get("visible_console", False)
            
            if visible_console:
                result = steamcmd.download_workshop_item(item_id, install_dir)
            else:
                from dayzconfigmaster.workshop.steamcmd import BaseSteamCmd
                steamcmd = BaseSteamCmd(str(steamcmd_path))
                result = steamcmd.download_workshop_item(item_id, install_dir)
            
            return {"success": result[0], "message": result[1]}
        
        elif action == "update":
            from dayzconfigmaster.workshop.steamcmd import ConsoleVisibleSteamCmd
            item_id = str(params.get("item_id", ""))
            result = ConsoleVisibleSteamCmd(str(self.projects_root / "steamcmd")).update_workshop_item(item_id)
            return {"success": result[0], "message": result[1]}
        
        elif action == "upload":
            from dayzconfigmaster.workshop.uploader import WorkshopUploader, WorkshopUploadRequest
            
            req = WorkshopUploadRequest(
                content_folder=params.get("content_folder", ""),
                title=params.get("title", ""),
                description=params.get("description", ""),
                preview_file=params.get("preview_file", ""),
                tags=params.get("tags", []),
                visibility=int(params.get("visibility", 0)),
                change_note=params.get("change_note", ""),
                published_file_id=params.get("published_file_id"),
            )
            
            uploader = WorkshopUploader()
            ok, msg, fid = uploader.publish(req)
            
            return {"success": ok, "message": msg, "published_file_id": fid}
        
        return {"error": "Unknown action"}


class MCPServer:
    """JSON-RPC server for MCP protocol."""
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
        self.methods: Dict[str, MCPMethod] = {
            "config": MCPConfigMethod(str(self.projects_root)),
            "build": MCPBuildMethod(str(self.projects_root)),
            "process": MCPProcessMethod(str(self.projects_root)),
            "workshop": MCPWorkshopMethod(str(self.projects_root))
        }
    
    def handle_request(self, request: dict) -> dict:
        """Handle a JSON-RPC request."""
        method_name = request.get("method", "")
        
        if "." in method_name:
            # Handle namespace methods like "config.list"
            parts = method_name.split(".")
            category = parts[0]
            action = parts[1]
            
            if category not in self.methods:
                return {
                    "jsonrpc": "2.0",
                    "error": {"code": -32601, "message": f"Method {method_name} not found"},
                    "id": request.get("id")
                }
            
            method = self.methods[category]
            
            if action == "list":
                # Special handling for list methods
                return {
                    "jsonrpc": "2.0",
                    "result": {"categories": list(self.methods.keys())},
                    "id": request.get("id")
                }
        
        if method_name not in self.methods:
            return {
                "jsonrpc": "2.0",
                "error": {"code": -32601, "message": f"Method {method_name} not found"},
                "id": request.get("id")
            }
        
        method = self.methods[method_name]
        params = request.get("params", {})
        
        try:
            result = asyncio.run(method.execute(params))
            
            return {
                "jsonrpc": "2.0",
                "result": result,
                "id": request.get("id")
            }
        
        except Exception as e:
            return {
                "jsonrpc": "2.0",
                "error": {"code": -32603, "message": str(e)},
                "id": request.get("id")
            }
    
    def list_methods(self) -> dict:
        """List all available methods."""
        return {
            "jsonrpc": "2.0",
            "result": {
                "methods": list(self.methods.keys())
            },
            "id": None
        }


def create_server(projects_root: str) -> MCPServer:
    """
    Create an MCP server instance.
    
    Args:
        projects_root: Path to DayZ projects directory
        
    Returns:
        MCPServer instance
    """
    return MCPServer(projects_root)


# HTTP endpoint for REST-style access
class MCPRestHandler:
    """REST handler for MCP endpoints."""
    
    def __init__(self, server: MCPServer):
        self.server = server
    
    async def handle(self, request_data: dict) -> dict:
        """Handle an HTTP request."""
        method = request_data.get("method", "")
        
        # Convert REST-style paths to method names
        if method == "/config" or "config" in method:
            return self.server.handle_request({
                "jsonrpc": "2.0",
                "method": "config.list",
                "params": {},
                "id": 1
            })
        
        elif method == "/build" or "build" in method:
            return self.server.handle_request({
                "jsonrpc": "2.0",
                "method": "build.list",
                "params": {},
                "id": 1
            })
        
        return {
            "error": {"code": -32601, "message": "Method not found"},
            "jsonrpc": "2.0"
        }


# ============== MCP Named Pipe Transport ==============

class MCPTransport(ABC):
    """Base class for MCP transport layers."""
    
    @abstractmethod
    def start(self, handler: callable) -> None:
        """Start the transport layer and handle incoming requests."""
        pass
    
    @abstractmethod
    def stop(self) -> None:
        """Stop the transport layer."""
        pass
    
    @abstractmethod
    def send_response(self, response: dict) -> bool:
        """Send a JSON-RPC response."""
        pass


class MCPNamedPipeTransport(MCPTransport):
    """
    Windows named pipe transport for MCP.
    
    Uses Windows Named Pipes for IPC between processes.
    """
    
    def __init__(self, pipe_name: str = "DayZConfigMaster"):
        self.pipe_name = f"\\\\.\\pipe\\{pipe_name}"
        self.running = False
        self.thread = None
        self.handler = None
    
    def start(self, handler: callable) -> None:
        """
        Start the named pipe server.
        
        Args:
            handler: Function to handle incoming JSON-RPC requests
        """
        if not HAS_WIN32:
            raise RuntimeError("Windows APIs not available")
        
        self.running = True
        self.handler = handler
        
        def accept_connections():
            while self.running:
                try:
                    # Create the named pipe
                    pipe = win32pipe.CreateNamedPipe(
                        self.pipe_name,
                        win32pipe.PIPE_ACCESS_DUPLEX,
                        win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
                        1, 8192, 8192,
                        0,
                        None
                    )
                    
                    # Wait for client connection
                    win32pipe.ConnectNamedPipe(pipe, None)
                    
                    # Read request
                    try:
                        _, request_data = win32file.ReadFile(pipe, 8192)
                        request_str = request_data.decode('utf-8').strip()
                        
                        if request_str:
                            # Parse and handle JSON-RPC
                            try:
                                request = json.loads(request_str)
                                response = self.handler(request)
                                
                                # Send response
                                response_bytes = json.dumps(response).encode('utf-8')
                                win32file.WriteFile(pipe, response_bytes)
                            except json.JSONDecodeError as e:
                                error_response = {
                                    "jsonrpc": "2.0",
                                    "error": {"code": -32700, "message": f"Parse error: {str(e)}"},
                                    "id": None
                                }
                                win32file.WriteFile(pipe, json.dumps(error_response).encode('utf-8'))
                    finally:
                        win32pipe.DisconnectNamedPipe(pipe)
                
                except pywintypes.error as e:
                    if self.running:
                        continue  # Ignore errors if still running
        
        self.thread = threading.Thread(target=accept_connections, daemon=True)
        self.thread.start()
    
    def stop(self) -> None:
        """Stop the named pipe server."""
        self.running = False
        if self.thread:
            self.thread.join(timeout=2.0)
    
    def send_response(self, response: dict) -> bool:
        """Send a JSON-RPC response - not used for server-side."""
        return True


class MCPUnixSocketTransport(MCPTransport):
    """
    Unix domain socket transport for MCP.
    
    Uses Unix domain sockets for IPC on Linux/macOS.
    """
    
    def __init__(self, socket_path: Optional[str] = None):
        self.socket_path = socket_path or os.path.join(
            tempfile.gettempdir(), "dayzconfigmaster.sock"
        )
        self.server_socket = None
        self.running = False
        self.thread = None
        self.handler = None
    
    def start(self, handler: callable) -> None:
        """
        Start the Unix socket server.
        
        Args:
            handler: Function to handle incoming JSON-RPC requests
        """
        if not HAS_UNIX_SOCKETS:
            raise RuntimeError("Unix sockets not available")
        
        # Remove old socket file if it exists
        if os.path.exists(self.socket_path):
            os.remove(self.socket_path)
        
        self.running = True
        self.handler = handler
        
        def accept_connections():
            self.server_socket = unix_socket.socket(unix_socket.AF_UNIX, unix_socket.SOCK_STREAM)
            self.server_socket.setsockopt(unix_socket.SOL_SOCKET, unix_socket.SO_REUSEADDR, 1)
            
            try:
                self.server_socket.bind(self.socket_path)
                self.server_socket.listen(5)
                
                while self.running:
                    try:
                        client_sock, client_addr = self.server_socket.accept()
                        
                        # Read request
                        request_data = client_sock.recv(8192)
                        if request_data:
                            try:
                                request = json.loads(request_data.decode('utf-8').strip())
                                response = self.handler(request)
                                
                                client_sock.sendall(json.dumps(response).encode('utf-8'))
                            except json.JSONDecodeError as e:
                                error_response = {
                                    "jsonrpc": "2.0",
                                    "error": {"code": -32700, "message": f"Parse error: {str(e)}"},
                                    "id": None
                                }
                                client_sock.sendall(json.dumps(error_response).encode('utf-8'))
                        
                        client_sock.close()
                    
                    except unix_socket.timeout:
                        continue
            
            finally:
                if self.server_socket:
                    self.server_socket.close()
                if os.path.exists(self.socket_path):
                    os.remove(self.socket_path)
        
        self.thread = threading.Thread(target=accept_connections, daemon=True)
        self.thread.start()
    
    def stop(self) -> None:
        """Stop the Unix socket server."""
        self.running = False
        if self.server_socket:
            try:
                self.server_socket.close()
            except Exception as exc:
                logger.debug("Failed to close socket: %s", exc)
        if self.thread:
            self.thread.join(timeout=2.0)
    
    def send_response(self, response: dict) -> bool:
        """Send a JSON-RPC response - not used for server-side."""
        return True


def create_transport(pipe_name: Optional[str] = None) -> MCPTransport:
    """
    Create the appropriate transport based on platform.
    
    Args:
        pipe_name: Named pipe name (Windows) or socket path
    
    Returns:
        Transport instance for the current platform
    """
    if os.name == 'nt':
        return MCPNamedPipeTransport(pipe_name)
    else:
        return MCPUnixSocketTransport(pipe_name)
