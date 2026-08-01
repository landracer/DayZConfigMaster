# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Discord bot integration for DayZ server management.

Provides Discord integration for:
- Remote server management via commands
- Status updates and notifications
- Player list queries
"""

import threading
from datetime import datetime
from enum import Enum
from pathlib import Path
from typing import Dict, List, Optional, Callable

import asyncio


class UserLevel(Enum):
    """User permission levels."""
    ADMIN = 'admin'        # Full access - everything
    MANAGE = 'manage'      # Management tasks (restarts, updates)
    MODERATE = 'moderate'  # Moderation tasks (messages, kicks, bans)
    VIEW = 'view'          # View-only (status, player list)


class AdminUser:
    """Represents an admin user with permissions."""
    
    def __init__(self, discord_user: str, level: UserLevel,
                 password: Optional[str] = None):
        """
        Initialize an admin user.
        
        Args:
            discord_user: Discord username (format: name#1234)
            level: Permission level
            password: Web interface password (optional)
        """
        self.discord_user = discord_user
        self.level = level
        self.password = password or ""
    
    def can(self, required_level: UserLevel) -> bool:
        """Check if user has sufficient permissions."""
        order = [UserLevel.VIEW, UserLevel.MODERATE, UserLevel.MANAGE, UserLevel.ADMIN]
        
        try:
            user_idx = order.index(self.level)
            req_idx = order.index(required_level)
            return user_idx >= req_idx
        except ValueError:
            return False


class DiscordBot:
    """
    Discord bot for DayZ server management.
    
    Features:
    - Server status commands
    - Player list queries
    - Kick/ban commands
    - Global messages
    
    Usage:
        bot = DiscordBot("your-token-here")
        bot.add_admin("admin#1234", UserLevel.ADMIN)
        bot.start()
    """
    
    def __init__(self, token: str, projects_root: Optional[str] = None):
        """
        Initialize the Discord bot.
        
        Args:
            token: Discord bot token
            projects_root: Root directory for project files
        """
        self.token = token
        self.projects_root = Path(projects_root or ".")
        
        # Admin users
        self._admins: Dict[str, AdminUser] = {}
        
        # Server callbacks (to be set by the application)
        self._callbacks = {
            'get_status': None,
            'send_message': None,
            'kick_player': None,
            'ban_player': None,
            'restart_server': None,
        }
        
        # Discord client (will be initialized when starting)
        self._client = None
        self._loop = None
        
        # Thread management
        self._thread: Optional[threading.Thread] = None
        self._running = False
    
    def add_admin(self, discord_user: str, level: UserLevel,
                  password: Optional[str] = None) -> AdminUser:
        """
        Add an admin user.
        
        Args:
            discord_user: Discord username (name#1234 format)
            level: Permission level
            password: Web interface password
            
        Returns:
            The created AdminUser
        """
        admin = AdminUser(discord_user, level, password)
        self._admins[discord_user] = admin
        return admin
    
    def remove_admin(self, discord_user: str) -> bool:
        """Remove an admin user."""
        if discord_user in self._admins:
            del self._admins[discord_user]
            return True
        return False
    
    def set_callback(self, callback_type: str,
                     callback: Optional[Callable]) -> bool:
        """
        Set a callback function for server operations.
        
        Args:
            callback_type: One of 'get_status', 'send_message',
                          'kick_player', 'ban_player', 'restart_server'
            callback: Callback function
            
        Returns:
            True if successful
        """
        if callback_type not in self._callbacks:
            return False
        
        self._callbacks[callback_type] = callback
        return True
    
    def _get_admin_for_user(self, discord_id: str) -> Optional[AdminUser]:
        """Get admin user by Discord ID/name."""
        # Try exact match first
        if discord_id in self._admins:
            return self._admins[discord_id]
        
        # Try partial match (without discriminator)
        for user, admin in self._admins.items():
            if discord_id.split('#')[0] == user.split('#')[0]:
                return admin
        
        return None
    
    def start(self):
        """Start the Discord bot."""
        import asyncio
        
        if self._running:
            return
        
        self._running = True
        self._thread = threading.Thread(target=self._run_async, daemon=True)
        self._thread.start()
    
    def stop(self):
        """Stop the Discord bot."""
        self._running = False
        if self._loop:
            self._loop.call_soon_threadsafe(self._loop.stop)
        
        if self._thread:
            self._thread.join(timeout=5)
            self._thread = None
    
    def _run_async(self):
        """Run the asyncio event loop."""
        import asyncio
        
        self._loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self._loop)
        
        try:
            self._loop.run_until_complete(self._start_bot())
        finally:
            self._loop.close()
    
    async def _start_bot(self):
        """Start the Discord bot (async)."""
        # Import here to avoid issues if discord.py not installed
        import discord
        
        intents = discord.Intents.default()
        intents.message_content = True
        intents.members = True
        
        self._client = discord.Client(intents=intents)
        
        @self._client.event
        async def on_ready():
            print(f'Discord bot logged in as {self._client.user}')
        
        @self._client.event
        async def on_message(message):
            await self._handle_message(message)
        
        # Run the bot (blocking)
        try:
            await self._client.start(self.token)
        except discord.LoginFailure:
            print("Failed to log in: Invalid token")
    
    async def _handle_message(self, message: discord.Message):
        """Handle an incoming Discord message."""
        # Ignore our own messages
        if message.author == self._client.user:
            return
        
        # Only respond to text channels (not DMs for simplicity)
        if not isinstance(message.channel, discord.TextChannel):
            return
        
        content = message.content.strip()
        
        # Check for command prefix
        if not content.startswith('!'):
            return
        
        # Extract command and arguments
        parts = content[1:].split()
        command = parts[0].lower() if parts else ''
        args = parts[1:] if len(parts) > 1 else []
        
        # Get user permissions
        admin = self._get_admin_for_user(f"{message.author.name}#{message.author.discriminator}")
        
        if admin is None:
            await message.channel.send("You are not authorized to use this bot.")
            return
        
        # Handle commands based on permission level
        try:
            if command == 'status':
                await self._cmd_status(message, admin)
            
            elif command == 'players' or command == 'playerlist':
                await self._cmd_players(message, admin)
            
            elif command == 'kick':
                await self._cmd_kick(message, admin, args)
            
            elif command == 'ban':
                await self._cmd_ban(message, admin, args)
            
            elif command == 'message' or command == 'say':
                await self._cmd_message(message, admin, args)
            
            elif command == 'restart':
                await self._cmd_restart(message, admin)
            
            elif command == 'help':
                await self._cmd_help(message, admin)
            
            else:
                await message.channel.send(f"Unknown command: {command}")
        
        except Exception as e:
            print(f"Command error: {e}")
            await message.channel.send(f"Error executing command: {str(e)}")
    
    async def _cmd_status(self, message: discord.Message, admin: AdminUser):
        """Handle !status command."""
        if not self._callbacks['get_status']:
            await message.channel.send("Status callback not configured.")
            return
        
        try:
            status = self._callbacks['get_status']()
            
            embed = discord.Embed(title="Server Status", color=0x00ff00)
            embed.add_field(name="Players Online", value=status.get('players', 0), inline=True)
            embed.add_field(name="CPU Usage", value=f"{status.get('cpu_percent', 0):.1f}%", inline=True)
            embed.add_field(name="Memory", value=f"{status.get('memory_mb', 0):.1f} MB", inline=True)
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error getting status: {e}")
    
    async def _cmd_players(self, message: discord.Message, admin: AdminUser):
        """Handle !players command."""
        if not self._callbacks['get_status']:
            await message.channel.send("Players callback not configured.")
            return
        
        try:
            status = self._callbacks['get_status']()
            players = status.get('player_list', [])
            
            if not players:
                await message.channel.send("No players currently online.")
                return
            
            # Format player list
            player_text = "\n".join([
                f"- {p.get('name', 'Unknown')}"
                for p in players[:10]  # Show max 10
            ])
            
            if len(players) > 10:
                player_text += f"\n... and {len(players) - 10} more"
            
            embed = discord.Embed(
                title=f"Players Online ({len(players)})",
                description=player_text,
                color=0x00ff00
            )
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error getting players: {e}")
    
    async def _cmd_kick(self, message: discord.Message, admin: AdminUser, args: List[str]):
        """Handle !kick command."""
        if not admin.can(UserLevel.MODERATE):
            await message.channel.send("You don't have permission to kick players.")
            return
        
        if len(args) < 1:
            await message.channel.send("Usage: !kick <player_name> [reason]")
            return
        
        player_name = args[0]
        reason = " ".join(args[1:]) if len(args) > 1 else ""
        
        if not self._callbacks['kick_player']:
            await message.channel.send("Kick callback not configured.")
            return
        
        try:
            success, msg = self._callbacks['kick_player'](player_name, reason)
            
            embed = discord.Embed(
                title="Kick Result",
                color=0xffaa00
            )
            
            if success:
                embed.add_field(name="Player", value=player_name, inline=True)
                embed.add_field(name="Reason", value=reason or "No reason specified", inline=True)
                embed.color = 0x00ff00
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error kicking player: {e}")
    
    async def _cmd_ban(self, message: discord.Message, admin: AdminUser, args: List[str]):
        """Handle !ban command."""
        if not admin.can(UserLevel.ADMIN):
            await message.channel.send("You don't have permission to ban players.")
            return
        
        if len(args) < 1:
            await message.channel.send("Usage: !ban <player_name> [duration_minutes] [reason]")
            return
        
        player_name = args[0]
        
        # Parse duration (numeric first arg)
        duration = 0
        reason_start = 1
        
        if len(args) > 1 and args[1].isdigit():
            duration = int(args[1])
            reason_start = 2
        
        reason = " ".join(args[reason_start:]) if len(args) > reason_start else ""
        
        if not self._callbacks['ban_player']:
            await message.channel.send("Ban callback not configured.")
            return
        
        try:
            success, msg = self._callbacks['ban_player'](player_name, duration, reason)
            
            embed = discord.Embed(
                title="Ban Result",
                color=0xff0000 if success else 0x00ff00
            )
            
            if success:
                embed.add_field(name="Player", value=player_name, inline=True)
                embed.add_field(name="Duration", value=f"{duration} minutes" if duration > 0 else "Permanent", inline=True)
                embed.color = 0xffaa00
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error banning player: {e}")
    
    async def _cmd_message(self, message: discord.Message, admin: AdminUser, args: List[str]):
        """Handle !message command."""
        if not admin.can(UserLevel.MODERATE):
            await message.channel.send("You don't have permission to send messages.")
            return
        
        if len(args) < 1:
            await message.channel.send("Usage: !message <message_text>")
            return
        
        msg = " ".join(args)
        
        if not self._callbacks['send_message']:
            await message.channel.send("Message callback not configured.")
            return
        
        try:
            success, _ = self._callbacks['send_message'](msg)
            
            embed = discord.Embed(
                title="Message Sent",
                description=msg,
                color=0x00ff00 if success else 0xff0000
            )
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error sending message: {e}")
    
    async def _cmd_restart(self, message: discord.Message, admin: AdminUser):
        """Handle !restart command."""
        if not admin.can(UserLevel.MANAGE):
            await message.channel.send("You don't have permission to restart the server.")
            return
        
        if not self._callbacks['restart_server']:
            await message.channel.send("Restart callback not configured.")
            return
        
        try:
            success, msg = self._callbacks['restart_server']()
            
            embed = discord.Embed(
                title="Restart Result",
                description=msg,
                color=0xffaa00
            )
            
            if success:
                embed.color = 0x00ff00
            
            await message.channel.send(embed=embed)
        
        except Exception as e:
            await message.channel.send(f"Error restarting server: {e}")
    
    async def _cmd_help(self, message: discord.Message, admin: AdminUser):
        """Handle !help command."""
        levels = {
            UserLevel.VIEW: ["status", "players"],
            UserLevel.MODERATE: ["kick", "message", "say"] + list(UserLevel.VIEW.value),
            UserLevel.MANAGE: ["restart"] + list(UserLevel.MODERATE.value),
            UserLevel.ADMIN: ["ban"] + list(UserLevel.MANAGE.value)
        }
        
        commands = levels.get(admin.level, [])
        cmd_list = "\n".join([f"!{c}" for c in sorted(commands)])
        
        embed = discord.Embed(
            title="Available Commands",
            description=f"**Permission Level:** {admin.level.value}",
            color=0x00ff00
        )
        
        embed.add_field(name="Commands", value=cmd_list, inline=False)
        await message.channel.send(embed=embed)


def create_default_admins() -> List[Dict]:
    """Create default admin users."""
    return [
        {
            "discord_user": "admin#0001",
            "level": UserLevel.ADMIN.value,
            "password": "admin"
        }
    ]