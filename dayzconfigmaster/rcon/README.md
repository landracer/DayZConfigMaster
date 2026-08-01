# RCon Module

RCon client library for DayZ server management.

## Features
- Connect to DayZ server via RCon protocol (port 2306)
- Authenticate with password
- Execute commands on the server
- Player list and management (kick, ban)
- Global messaging

## Usage

```python
from dayzconfigmaster.rcon import RConClient, RConError

# Create client
client = RConClient(
    host="localhost",
    port=2306,
    password="rcon_password"
)

try:
    # Connect and authenticate
    if not client.connect():
        print("Failed to connect")
    
    if not client.authenticate():
        print("Authentication failed")
    
    # Get player list
    players = client.get_player_list()
    for player in players:
        print(f"Player: {player['name']}, Ping: {player['ping']}")
    
    # Kick a player
    success = client.kick_player("BadPlayer", "Rule violation")
    
    # Send global message
    success = client.send_global_message("Server restart in 5 minutes!")
    
except RConError as e:
    print(f"RCon error: {e}")

finally:
    client.disconnect()
```

## Commands

The following commands are available:

| Command | Description |
|---------|-------------|
| `get_player_list()` | Get connected players |
| `kick_player(name, reason)` | Kick a player |
| `ban_player(name, duration, reason)` | Ban a player |
| `send_global_message(msg)` | Send global chat message |
| `server_shutdown(delay)` | Schedule server shutdown |

## Requirements

The DayZ server must have RCon enabled. This requires:
- The Server Admin addon installed
- Or use the dedicated RCon port (2306) in newer versions