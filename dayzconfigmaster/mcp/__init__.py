# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# MCP package for DayZConfigMaster

"""
Model Context Protocol server package.

Provides JSON-RPC interface for AI agent automation.
"""

from .server import MCPServer, create_server, MCPRestHandler

__all__ = ['MCPServer', 'create_server', 'MCPRestHandler']