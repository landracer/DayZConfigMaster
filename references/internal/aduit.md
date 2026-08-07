aduit our work. these are other suggestions to properly making multi-instance work. I think our app is VERY bloated. BUT has lots of useful feature to make all this come togther properly. Below are ssever suggestion for others on how to do this.. I prefer the script style



To add mods, you do **not** edit the `serverDZ.cfg` file directly. Instead, you specify mods in the **startup command** using the `-mod=` and `-serverMod=` parameters. The `serverDZ.cfg` file remains largely the same, with one critical exception: `verifySignatures`.

### 1. The Startup Command (Where Mods Are Loaded)
You must append the `-mod` flag to your launch command. Mods are separated by semicolons (`;`). On Linux, if you downloaded mods via SteamCMD, you typically reference them by their **Workshop ID** or by linking them to a folder name prefixed with `@`.

**Example Command for Instance 1 (with Mods):**
```bash
./DayZServer -config=serverDZ.cfg -port=2302 -profiles=/home/dayz/server1/profiles "-mod=1559212036;1564026768" -dologs -adminlog
```
*   **`-mod=`**: Loads client-side mods (visible to players, e.g., maps, weapons, scripts).
*   **`-serverMod=`**: Loads server-side only mods (e.g., admin tools like VPP or DZSA-ServerInfo that don't need to be downloaded by players).

**Using Folder Names (Common Practice):**
Most admins create symbolic links to the Workshop folders and name them (e.g., `@CF`, `@Expansion`). The command then looks like this:
```bash
./DayZServer -config=serverDZ.cfg -port=2302 -profiles=/home/dayz/server1/profiles "-mod=@CF;@Expansion;@MyMap" -dologs -adminlog
```

### 2. Required `serverDZ.cfg` Changes
While you don't list mod names in `serverDZ.cfg`, you must adjust **`verifySignatures`** to allow mods to load correctly.

*   **`verifySignatures = 2;`** (Recommended): Enforces strict signature checking. All mods **must** be properly signed and match the workshop version exactly. This is required for most public modded servers.
*   **`verifySignatures = 0;`**: Disables signature checking. Only use this for local testing with unsigned/custom mods. **Do not use on public servers** as it allows hacked clients.

**Example `serverDZ.cfg` snippet for a modded server:**
```c
hostname = "My Modded Server #1";
password = "";
passwordAdmin = "YourAdminPassword";
maxPlayers = 60;

// CRITICAL FOR MODS:
verifySignatures = 2;  // Set to 2 for signed workshop mods
forceSameBuild = 1;    // Ensures clients match server version

// ... rest of your config (ports, instanceId, etc.)
instanceId = 1;
steamQueryPort = 27016;

class Missions
{
    class DayZ
    {
        // If using a modded map, this template name must match the mod's mission folder
        template = "dayzOffline.chernarusplus"; 
    };
};
```

### 3. Multi-Instance Setup with Mods
For your 3 instances, you simply add the unique `-mod` string to each specific startup command or systemd service file.

**Systemd Service Example (`/etc/systemd/system/dayz-server1.service`):**
```ini
[Service]
User=dayz
WorkingDirectory=/home/dayz/server1
# Add the -mod parameter here with your specific mod IDs or @FolderNames
ExecStart=/home/dayz/server1/DayZServer -config=serverDZ.cfg -port=2302 -profiles=/home/dayz/server1/profiles "-mod=@CF;@Expansion" -dologs -adminlog
Restart=on-failure
```

**Key Steps for Each Instance:**
1.  **Download Mods:** Use SteamCMD to download the workshop items to a central location or directly into each server folder.
2.  **Copy Keys:** Copy the `.bikey` files from every mod's `keys` folder into your server's main `keys` directory (e.g., `/home/dayz/server1/keys/`). If keys are missing, the server will fail to start.
3.  **Update Command:** Add the `-mod="ID;ID"` or `-mod="@Folder;@Folder"` string to the startup command for that specific instance.
4.  **Firewall:** No extra ports are needed for mods; the existing Game and Query ports handle all modded traffic.

OR **



To automate everything, you can use a unified bash script that handles updating the game, downloading mods, symlinking mod folders, syncing .bikey security keys, and restarting all three services.
Using symbolic links (symlinks) allows you to download a mod only once but share it across all three server instances effortlessly, saving massive amounts of disk space.
## Step 1: The Master Automation Script
Log in as your dayz user, create a script named manage_servers.sh in your home directory, and paste the code below.

nano /home/dayz/manage_servers.sh

#!/bin/bash
# --- CONFIGURATION ---
STEAMCMD_DIR="/home/dayz/steamcmd"
BASE_SERVER_DIR="/home/dayz/servers/dayz-inst1" # Primary installation dir
STEAM_USER="YOUR_STEAM_USERNAME"
STEAM_PASS="YOUR_STEAM_PASSWORD"
# List your Steam Workshop Mod IDs here (separated by spaces)# Example: 1559212036 (CF), 2276010185 (VPPAdminTools)
MOD_IDS=(1559212036 2276010185)
# Array of all your instance paths
INSTANCES=(
    "/home/dayz/servers/dayz-inst1"
    "/home/dayz/servers/dayz-inst2"
    "/home/dayz/servers/dayz-inst3"
)# ---------------------

echo "==> Stopping DayZ Services..."
sudo systemctl stop dayz-inst1 dayz-inst2 dayz-inst3

echo "==> Updating DayZ Base Server and Downloading Mods..."# Build the SteamCMD command dynamically
STEAM_CMD="./steamcmd.sh +login $STEAM_USER $STEAM_PASS +force_install_dir $BASE_SERVER_DIR +app_update 223350 validate"for MOD_ID in "${MOD_IDS[@]}"; do
    STEAM_CMD="$STEAM_CMD +workshop_download_item 221100 $MOD_ID validate"done
STEAM_CMD="$STEAM_CMD +quit"
# Run SteamCMD
cd "$STEAMCMD_DIR" || exiteval "$STEAM_CMD"

echo "==> Syncing Base Files and Mods to All Instances..."for INSTANCE in "${INSTANCES[@]}"; do
    # Skip instance 1 since it's our base download folder
    if [ "$INSTANCE" != "$BASE_SERVER_DIR" ]; then
        # Sync core game files efficiently (excludes configs/profiles so they aren't overwritten)
        rsync -av --delete --exclude='serverDZ.cfg' --exclude='profiles/' --exclude='mpmissions/' --exclude='*.log' "$BASE_SERVER_DIR/" "$INSTANCE/"
    fi

    # Create the keys folder if it doesn't exist
    mkdir -p "$INSTANCE/keys"

    # Link downloaded mods and copy their bikeys
    for MOD_ID in "${MOD_IDS[@]}"; do
        WORKSHOP_DIR="$BASE_SERVER_DIR/steamapps/workshop/content/221100/$MOD_ID"
        
        # Determine mod folder name from meta.cpp or fall back to ID
        MOD_NAME=$(grep -oP 'name = "\K[^"]+' "$WORKSHOP_DIR/meta.cpp" 2>/dev/null | tr -d ' ' | tr -cd '[:alnum:]_-')
        [ -z "$MOD_NAME" ] && MOD_NAME="mod_$MOD_ID"
        MOD_DIR_NAME="@$MOD_NAME"

        echo "    Configuring $MOD_DIR_NAME for $(basename "$INSTANCE")"

        # Create symlink for the mod folder
        rm -rf "$INSTANCE/$MOD_DIR_NAME"
        ln -s "$WORKSHOP_DIR" "$INSTANCE/$MOD_DIR_NAME"

        # Linux file system fix: DayZ requires lowercase keys and addons
        # Copy security keys to instance keys folder
        cp "$WORKSHOP_DIR/[Kk][Ee][Yy][Ss]"/*.bikey "$INSTANCE/keys/" 2>/dev/null
        cp "$WORKSHOP_DIR/"*.bikey "$INSTANCE/keys/" 2>/dev/null
    donedone

echo "==> Restarting DayZ Services..."
sudo systemctl daemon-reload
sudo systemctl start dayz-inst1 dayz-inst2 dayz-inst3

echo "==> All 3 instances are fully updated and running!"

Make the script executable:

chmod +x /home/dayz/manage_servers.sh

------------------------------
## Step 2: The Scripted systemd Configuration
To make the -mod flags completely hands-off, update your systemd service files to load all directories starting with @ dynamically via a wildcard. This means you never have to change your service files again when adding or removing mods.
Open each service file (e.g., sudo nano /etc/systemd/system/dayz-inst1.service) and set the ExecStart line to use this dynamic bash loop:
## /etc/systemd/system/dayz-inst1.service

[Service]
User=dayz
Group=dayz
WorkingDirectory=/home/dayz/servers/dayz-inst1
ExecStart=/bin/bash -c 'MODS=$(ls -d @* 2>/dev/null | tr "\\\\n" ";"); /home/dayz/servers/dayz-inst1/DayZServer -config=serverDZ.cfg -port=2302 -profiles=profiles -dologs -adminlog -netlog "-mod=$MODS"'
Restart=on-failure

## /etc/systemd/system/dayz-inst2.service

[Service]
User=dayz
Group=dayz
WorkingDirectory=/home/dayz/servers/dayz-inst2
ExecStart=/bin/bash -c 'MODS=$(ls -d @* 2>/dev/null | tr "\\\\n" ";"); /home/dayz/servers/dayz-inst2/DayZServer -config=serverDZ.cfg -port=2303 -profiles=profiles -dologs -adminlog -netlog "-mod=$MODS"'
Restart=on-failure

## /etc/systemd/system/dayz-inst3.service

[Service]
User=dayz
Group=dayz
WorkingDirectory=/home/dayz/servers/dayz-inst3
ExecStart=/bin/bash -c 'MODS=$(ls -d @* 2>/dev/null | tr "\\\\n" ";"); /home/dayz/servers/dayz-inst3/DayZServer -config=serverDZ.cfg -port=2304 -profiles=profiles -dologs -adminlog -netlog "-mod=$MODS"'
Restart=on-failure

------------------------------
## Step 3: Giving dayz User Rights to Manage Services
Because the maintenance script runs under the dayz user but needs to control system services (sudo systemctl), you should allow it to do so without prompting for a password.

   1. Open the sudoers configuration:
   
   sudo visudo
   
   2. Add this exact line at the very bottom of the file:
   
   dayz ALL=(ALL) NOPASSWD: /usr/bin/systemctl stop dayz-inst1 dayz-inst2 dayz-inst3, /usr/bin/systemctl start dayz-inst1 dayz-inst2 dayz-inst3, /usr/bin/systemctl daemon-reload
   
   
## How to use it going forward:

   1. Open /home/dayz/manage_servers.sh.
   2. Simply add or remove Steam Workshop IDs from the MOD_IDS=(...) line.
   3. Run ./manage_servers.sh. The script will stop the servers, fetch updates, build the symlinks, move the security keys, and boot all three instances back up automatically.



OR**


To add mods to your DayZ instances, you do not modify the server's .cfg files (serverDZ.cfg). Instead, you download the mod files into your server directory and append them to the launch command inside your systemd service files using the -mod parameter. [1, 2] 
Here is exactly how to set it up for your instances.
## Step 1: How the Server Mod Structure Looks
When you download mods (typically via SteamCMD using the workshop ID), they must be placed in the root directory of your instance (e.g., /home/dayz/servers/dayz-inst1/). [3] 
They must be named with an @ symbol prefix. Your instance directory will look like this: [4] 

/home/dayz/servers/dayz-inst1/
├── @CF/
├── @Community-Online-Tools/
├── DayZServer (executable)
└── serverDZ.cfg

------------------------------
## Step 2: Update Your Systemd Service Files
You must inject the mods into the ExecStart line of your systemd configuration. Separate multiple mods with a semicolon (;). [5] 
Run sudo nano /etc/systemd/system/dayz-inst1.service and update the ExecStart line to look like this:

[Service]
User=dayz
Group=dayz
WorkingDirectory=/home/dayz/servers/dayz-inst1
ExecStart=/home/dayz/servers/dayz-inst1/DayZServer -config=serverDZ.cfg -port=2302 -profiles=profiles -dologs -adminlog -netlog "-mod=@CF;@Community-Online-Tools;@VPPAdminTools"
Restart=on-failure

(Note: Wrapping the -mod="..." part in double quotes is highly recommended in Linux systemd files to prevent the semicolons from breaking the systemd parser).
------------------------------
## Step 3: Don't Forget the Keys!
If you only add the -mod parameter, players will get kicked for an "Invalid Version" or "Modified Data" error. You must copy the authorization keys from the mod folders into the server's central keys folder.
For every mod you add, run this copy command:

cp /home/dayz/servers/dayz-inst1/@ModName/keys/*.bikey /home/dayz/servers/dayz-inst1/keys/

------------------------------
## Step 4: Reload and Restart
Every time you change a systemd service file or update a mod, you need to tell Linux to refresh and restart the specific instance:

sudo systemctl daemon-reload
sudo systemctl restart dayz-inst1

If you want, tell me:

* Do you want to use the same exact mods across all 3 instances, or will they be different styles (e.g., one PvE, one PvP)?
* Would you like an automated bash script that handles downloading the mods from the Steam Workshop and copying the .bikey files automatically? [6] 

I can tailor a script to make managing mods across 3 separate instances completely hands-off.

[1] [https://www.reddit.com](https://www.reddit.com/r/valheim/comments/mv24fw/configure_mods_in_vortex/)
[2] [https://www.youtube.com](https://www.youtube.com/watch?v=07fBjXyMYRE)
[3] [https://apexminecrafthosting.com](https://apexminecrafthosting.com/guides/minecraft/control-panel/how-to-add-mods/)
[4] [https://nodecraft.com](https://nodecraft.com/support/games/arma3/modding-your-arma-3-nodecraft-server)
[5] [https://steamcommunity.com](https://steamcommunity.com/sharedfiles/filedetails/?id=2718469431)
[6] [https://www.reddit.com](https://www.reddit.com/r/CreateMod/comments/1nsndm6/if_i_make_a_schematic_in_a_singleplayer_world/)


OR**

To keep your firewall configuration as tight and organized as possible, group your ports sequentially. DayZ uses two primary ports per instance: the Game Port (UDP) and the Steam Query Port (UDP). [1, 2] 
The most compact layout places all game ports together and all query ports together. This allows you to open narrow, contiguous ranges in your firewall.
## Port Allocation Map

| Instance | Game Port (UDP) | Steam Query Port (UDP) |
|---|---|---|
| Instance 1 | 2302 [1] | 27016 [1] |
| Instance 2 | 2303 | 27017 |
| Instance 3 | 2304 | 27018 |

------------------------------
## Step 1: Update Server Config Files
For each instance, update its specific serverDZ.cfg file with its assigned ports.
## Instance 1 (/home/dayz/servers/dayz-inst1/serverDZ.cfg)

port = 2302;
steamQueryPort = 27016;

## Instance 2 (/home/dayz/servers/dayz-inst2/serverDZ.cfg)

port = 2303;
steamQueryPort = 27017;

## Instance 3 (/home/dayz/servers/dayz-inst3/serverDZ.cfg)

port = 2304;
steamQueryPort = 27018;

------------------------------
## Step 2: Configure Systemd Services
Create or edit your systemd service files to pass the exact game ports via the startup arguments.
## /etc/systemd/system/dayz-inst1.service

[Service]
...
WorkingDirectory=/home/dayz/servers/dayz-inst1
ExecStart=/home/dayz/servers/dayz-inst1/DayZServer -config=serverDZ.cfg -port=2302 -profiles=profiles -dologs -adminlog -netlog
...

## /etc/systemd/system/dayz-inst2.service

[Service]
...
WorkingDirectory=/home/dayz/servers/dayz-inst2
ExecStart=/home/dayz/servers/dayz-inst2/DayZServer -config=serverDZ.cfg -port=2303 -profiles=profiles -dologs -adminlog -netlog
...

## /etc/systemd/system/dayz-inst3.service

[Service]
...
WorkingDirectory=/home/dayz/servers/dayz-inst3
ExecStart=/home/dayz/servers/dayz-inst3/DayZServer -config=serverDZ.cfg -port=2304 -profiles=profiles -dologs -adminlog -netlog
...

------------------------------
## Step 3: Tighten Your Firewall (UFW Example)
Because the ports are perfectly sequential, you only need two tight rules to cover all three instances completely.
Run these commands to open the exact ranges:

# Open Game Ports for Instances 1, 2, and 3
sudo ufw allow 2302:2304/udp
# Open Steam Query Ports for Instances 1, 2, and 3
sudo ufw allow 27016:27018/udp

------------------------------
## Step 4: Apply and Run
Reload systemd to read your third instance and your port modifications, then start everything up:

sudo systemctl daemon-reload
# Start and enable all three instances
sudo systemctl enable --now dayz-inst1
sudo systemctl enable --now dayz-inst2
sudo systemctl enable --now dayz-inst3

If you are interested, I can show you how to set up:

* A unified bash script to update all 3 instances via SteamCMD at once
* Individual CPU affinity (taskset) settings in systemd so the instances don't choke each other on the same CPU cores


[1] [https://pcidssguide.com](https://pcidssguide.com/firewall-rule-configuration-best-practices/)
[2] [https://arkascendedservermanager.com](https://arkascendedservermanager.com/opening-windows-firewall-ports/)

OR**

To run **3 instances** with the **minimum number of open firewall ports**, you can configure them to use adjacent ports. Each instance strictly requires only **2 UDP ports**: one for the game connection and one for the Steam query (server list visibility).

You do **not** need to open ranges (e.g., 2302–2305); opening the single specific Game Port and single Query Port for each instance is sufficient.

### 1. Minimal Port Allocation Plan
This configuration uses a tight block of **6 total UDP ports**.

| Instance | Game Port (UDP) | Steam Query Port (UDP) | Firewall Rule Needed |
| :--- | :--- | :--- | :--- |
| **Server 1** | `2302` | `27016` | `2302/udp`, `27016/udp` |
| **Server 2** | `2303` | `27017` | `2303/udp`, `27017/udp` |
| **Server 3** | `2304` | `27018` | `2304/udp`, `27018/udp` |

*Note: While the game engine may internally utilize adjacent ports for some traffic, only the **Game Port** and **Steam Query Port** must be explicitly opened in the firewall for external connections to work.*

### 2. Firewall Commands (openSUSE)
Run these commands to open exactly these 6 ports permanently:

```bash
# Server 1
sudo firewall-cmd --zone=public --permanent --add-port=2302/udp
sudo firewall-cmd --zone=public --permanent --add-port=27016/udp

# Server 2
sudo firewall-cmd --zone=public --permanent --add-port=2303/udp
sudo firewall-cmd --zone=public --permanent --add-port=27017/udp

# Server 3
sudo firewall-cmd --zone=public --permanent --add-port=2304/udp
sudo firewall-cmd --zone=public --permanent --add-port=27018/udp

# Apply changes
sudo firewall-cmd --reload
```

### 3. Critical Configuration Changes
For this tight port spacing to work, you must manually configure each instance's `serverDZ.cfg` file. The `steamQueryPort` is **not** automatic when running multiple instances.

**Instance 1 (`serverDZ.cfg`):**
```c
instanceId = 1;
steamQueryPort = 27016; // Must match firewall
```
*Startup:* `-port=2302`

**Instance 2 (`serverDZ.cfg`):**
```c
instanceId = 2;
steamQueryPort = 27017; // Must match firewall
```
*Startup:* `-port=2303`

**Instance 3 (`serverDZ.cfg`):**
```c
instanceId = 3;
steamQueryPort = 27018; // Must match firewall
```
*Startup:* `-port=2304`

### 4. Verification
After starting all three servers, check the `.RPT` log file for each instance. You should see a line similar to:
`SUCCESS: SteamGameServer_Init(..., 2303, 27017, ...)`
If the ports in the log match your firewall rules and `serverDZ.cfg`, the instances will be visible and connectable.


Check how custom maps are worked with

1. Obtain Map Files
Unlike stock maps, you need two components:

Server Mod Files: The actual map data (.pbo files). You can download these from the map author's website, GitHub, or extract them from the Steam Workshop folder (steamapps/workshop/content/221100/<MapID>) if you have the map subscribed on a Windows machine. 
Mission Folder: A specific folder (e.g., dayzOffline.namalsk or empty.deerisle) that must be placed in the mpmissions directory. 

DayZ custom map server files download Namalsk Deer Isle

View all
2. Directory Structure
For your custom map instance (e.g., Instance #3), the structure should look like this:

/home/dayz/server3/
├── DayZServer              # Symlink or binary
├── serverDZ.cfg            # Config pointing to custom map
├── profiles/               # Logs and persistence (storage_3)
├── @CustomMapMod/          # The map mod folder (e.g., @Namalsk)
│   └── keys/               # CRITICAL: .bikey files must be here
└── mpmissions/
    └── dayzOffline.namalsk # The specific mission folder for the map

Critical Requirement: You must copy the .bikey signature files from the mod's keys folder into your server's main keys folder (or ensure the server is launched with -mod=@CustomMapMod so it reads the keys from there). Without these, the server will fail to start with a "Signature verification failed" error. 

3. Configuration (serverDZ.cfg)
Edit the serverDZ.cfg for this instance. The template line must match the folder name inside mpmissions exactly. 

// serverDZ.cfg for Custom Map
hostname = "My Custom Map Server";
steamQueryPort = 27018; // Unique port for Instance 3
instanceId = 3;         // Unique persistence folder

class Missions {
    class DayZ {
        template = "dayzOffline.namalsk"; // Must match mpmissions folder name
    };
};




