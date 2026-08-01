# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
RCon client for DayZ server management.

Implements the RCon protocol for DayZ servers:
- Authentication with password
- Execute RCon commands
- Player list and management
"""

import errno
import socket
import struct
from typing import List, Optional, Dict, Any


class RConError(Exception):
    """Raised when an RCon operation fails."""
    pass


class RConClient:
    """
    RCon client for DayZ server management.

    Implements the basic RCon protocol used by DayZ servers. The client is
    designed to be created per-instance and will reconnect automatically on a
    broken pipe or transient failure.

    Note: DayZ requires the Server Admin addon with RCon enabled,
    or uses a dedicated RCon port (2306) in newer versions.
    """

    # Packet types
    SERVER_AUTH = 0
    SERVER_EXEC_COMMAND = 1
    SERVER_RESPONSE_AUTH = 2
    SERVER_RESPONSE_VALUE = 3

    def __init__(self, host: str = "localhost", port: int = 2306,
                 password: str = "rcon"):
        """
        Initialize the RCon client.

        Args:
            host: Server hostname or IP
            port: RCon port (default 2306)
            password: RCon password
        """
        self.host = host
        self.port = port
        self.password = password

        self._socket: Optional[socket.socket] = None
        self._authenticated = False

    def connect(self) -> bool:
        """Connect to the server."""
        self.disconnect()
        try:
            self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._socket.settimeout(10.0)
            self._socket.connect((self.host, self.port))
            return True
        except Exception as e:
            self._socket = None
            raise RConError(f"Failed to connect to {self.host}:{self.port}: {e}")

    def disconnect(self):
        """Disconnect from the server."""
        if self._socket:
            try:
                self._socket.close()
            except Exception:
                pass
            self._socket = None
        self._authenticated = False

    def authenticate(self) -> bool:
        """
        Authenticate with the RCon server.

        Returns:
            True if authentication successful

        Raises:
            RConError: If authentication fails
        """
        if not self._socket:
            self.connect()

        # Send authentication packet
        request_id = 1
        packet = self._create_packet(request_id, self.SERVER_AUTH, self.password)
        self._send_packet(packet)

        # Receive response
        response_id, response_type, response_data = self._receive_packet()

        if response_type != self.SERVER_RESPONSE_AUTH:
            raise RConError("Authentication failed")

        self._authenticated = True
        return True

    def _ensure_authenticated(self) -> None:
        """Connect and authenticate if not already authenticated."""
        if not self._socket:
            self.connect()
        if not self._authenticated:
            self.authenticate()

    def execute_command(self, command: str) -> str:
        """
        Execute an RCon command.

        Args:
            command: The command to execute

        Returns:
            Command output from server

        Raises:
            RConError: If command execution fails
        """
        self._ensure_authenticated()

        # Send command packet
        request_id = 2
        packet = self._create_packet(request_id, self.SERVER_EXEC_COMMAND, command)

        try:
            self._send_packet(packet)
        except (BrokenPipeError, OSError, RConError) as exc:
            # Connection lost; try one reconnect before giving up.
            if isinstance(exc, OSError) and exc.errno != errno.EPIPE:
                raise
            self.disconnect()
            self._ensure_authenticated()
            self._send_packet(packet)

        # Receive response (may be split into multiple packets)
        responses = []
        max_packets = 1000  # Safety break to avoid infinite loops.

        for _ in range(max_packets):
            try:
                response_id, response_type, response_data = self._receive_packet()

                if response_type == self.SERVER_RESPONSE_VALUE:
                    responses.append(response_data)
                elif response_type == self.SERVER_AUTH:  # New packet start
                    break
                else:
                    break

            except socket.timeout:
                break

        return "\n".join(responses)

    # Alias used by the GUI and external callers.
    send_command = execute_command
    
    def get_player_list(self) -> List[Dict[str, Any]]:
        """
        Get a list of connected players.
        
        Returns:
            List of player dictionaries with: name, steam_id, ping
            
        Raises:
            RConError: If command fails
        """
        # The actual DayZ RCon command format may vary
        response = self.execute_command("players")
        
        players = []
        for line in response.split("\n"):
            if not line.strip():
                continue
            
            # Parse player line - format may vary
            parts = line.split()
            if len(parts) >= 3:
                try:
                    players.append({
                        "name": parts[1] if len(parts) > 1 else "Unknown",
                        "ping": int(parts[2]) if parts[2].isdigit() else 0,
                        "steam_id": ""
                    })
                except ValueError:
                    continue
        
        return players
    
    def kick_player(self, player_name: str, reason: str = "") -> bool:
        """
        Kick a player from the server.
        
        Args:
            player_name: Name of the player to kick
            reason: Reason for kicking
            
        Returns:
            True if successful
            
        Raises:
            RConError: If command fails
        """
        cmd = f"kick {player_name}"
        if reason:
            cmd += f' "{reason}"'
        
        self.execute_command(cmd)
        return True
    
    def ban_player(self, player_name: str, duration: int = 0,
                   reason: str = "") -> bool:
        """
        Ban a player from the server.
        
        Args:
            player_name: Name of the player to ban
            duration: Duration in minutes (0 = permanent)
            reason: Reason for banning
            
        Returns:
            True if successful
            
        Raises:
            RConError: If command fails
        """
        cmd = f"ban {player_name}"
        if duration > 0:
            cmd += f" {duration}"
        if reason:
            cmd += f' "{reason}"'
        
        self.execute_command(cmd)
        return True
    
    def send_global_message(self, message: str) -> bool:
        """
        Send a global message to all players.
        
        Args:
            message: Message to send
            
        Returns:
            True if successful
            
        Raises:
            RConError: If command fails
        """
        self.execute_command(f'say "{message}"')
        return True
    
    def server_shutdown(self, delay: int = 0) -> bool:
        """
        Schedule a server shutdown.
        
        Args:
            delay: Delay in seconds before shutdown
            
        Returns:
            True if successful
            
        Raises:
            RConError: If command fails
        """
        self.execute_command(f"shutdown {delay}")
        return True
    
    def _create_packet(self, request_id: int, packet_type: int,
                       data: str = "") -> bytes:
        """Create an RCon packet."""
        # Packet format:
        # [0-3]:   Request ID (4 bytes)
        # [4-7]:   Packet type (4 bytes)  
        # [8-...]: Data + null terminator
        # [...-..]: Empty string terminator
        
        data_bytes = data.encode('utf-8')
        
        packet = struct.pack('<ii', request_id, packet_type)
        packet += data_bytes
        packet += b'\x00'  # Null terminator for data
        packet += b'\x00\x00'  # Empty string terminator
        
        return packet
    
    def _send_packet(self, packet: bytes):
        """Send a packet to the server."""
        if not self._socket:
            raise RConError("Not connected")
        
        # Add length prefix (4 bytes)
        length = len(packet)
        header = struct.pack('<i', length)
        
        self._socket.sendall(header + packet)
    
    def _receive_packet(self) -> tuple:
        """Receive a packet from the server."""
        if not self._socket:
            raise RConError("Not connected")
        
        # First 4 bytes: packet length
        header = self._recv_all(4)
        if not header:
            raise RConError("Connection closed")
        
        length = struct.unpack('<i', header)[0]
        
        # Receive the packet data
        data = self._recv_all(length)
        
        if len(data) < 8:  # Minimum: request_id + type
            raise RConError("Invalid packet")
        
        # Parse packet
        request_id, packet_type = struct.unpack('<ii', data[:8])
        response_data = data[8:].rstrip(b'\x00').decode('utf-8')
        
        return request_id, packet_type, response_data
    
    def _recv_all(self, length: int) -> bytes:
        """Receive exactly 'length' bytes."""
        data = b''
        while len(data) < length:
            try:
                chunk = self._socket.recv(length - len(data))
            except socket.timeout:
                raise RConError("Socket timeout while receiving data")
            if not chunk:
                raise RConError("Connection closed unexpectedly")
            data += chunk
        return data


def test_rcon_connection(host: str = "localhost", port: int = 2306,
                         password: str = "rcon") -> tuple:
    """
    Test RCon connection.
    
    Args:
        host: Server hostname or IP
        port: RCon port
        password: RCon password
        
    Returns:
        Tuple of (success, message)
    """
    client = None
    
    try:
        client = RConClient(host, port, password)
        
        if not client.connect():
            return False, "Connection failed"
        
        if not client.authenticate():
            return False, "Authentication failed"
        
        # Test a simple command
        response = client.execute_command("players")
        
        return True, f"Connected successfully. Response: {response[:100]}..."
        
    except RConError as e:
        return False, str(e)
    
    finally:
        if client:
            client.disconnect()