// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/clientkickedmodule.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EClientKicked
{
    UNKNOWN = -1,       // -1 must always be UNKNOWN
    OK = 0,             // 0 must always be OK
    
    SERVER_EXIT,        // Server last ditch effort to try and disconnect players
    KICK_ALL_ADMIN,     // Admin kicked all (RCON)
    KICK_ALL_SERVER,    // Server kicked all
    
    TIMEOUT,            // Players network timed out
    LOGOUT,             // Player logged out (does not need a message as it is not an &quot;error&quot;)
    KICK,               // Generic kick 
    BAN,                // Player was banned
    PING,               // Player went over ping limit
    MODIFIED_DATA,      // Player has modified data or has a corrupted install
    UNSTABLE_NETWORK,   // Player internet is too unstable
    SERVER_SHUTDOWN,    // Server is shutting down and therefore kicks all players  
    
    NOT_WHITELISTED,    // Player is not whitelisted
    NO_IDENTITY,        // No identity received while creating identity
    NO_INPUT_INTERFACE, // No input interface exists for the player 
    INVALID_UID,        // UID was incorrect while creating identity
    BANK_COUNT,         // Amount of banks was changed
    ADMIN_KICK,         // Kicked by admin
    INVALID_ID,         // Player has an invalid ID
    
    INPUT_HACK,         // Player is sending more Inputs than possible
    
    QUIT,               // Player closed the game
    LEAVE,              // Player pressed the Leave button
 
    PLATFORM_NOT_SUPPORTED, // Player&#39;s platform is not allowed on this server (crossplatform restriction)
 
    // LoginMachine kicks (0x0030)
    LOGIN_MACHINE_ERROR = 48,   // Generic LoginMachine error (fallback)
    PLAYER_STATE_TIMEOUT,       // Player spent too much time in one state of the LoginMachine
    IDENTITY_NOT_CREATED,       // Player was assigned but identity was not yet created
    DUPLICATE_UID,              // Player tried to log in while not being logged out yet
    DB_PING_RESPONSE,           // Unknown/unexpected database response
    DB_IDENTITY,                // Identity not found
    DB_TIMEOUT,                 // Database connection timed out
    DB_BAD_VERSION,             // Unsupported version of character data
    DB_ERROR,                   // Database error
    DB_APP_ERROR,               // Something went wrong on server
    DB_APP_EXIT,                // Application is exiting
    DB_BAD_DATA,                // Character data check failed
    DB_GET_RESPONSE,            // Unknown/unexpected database response
    NO_PLAYER,                  // Player was not created on server
    CREATE_NEW_CLIENT,          // Creating of the client with a new character failed
    CREATE_LOADED_CHAR,         // Creating the existing character failed
    LOAD_LOADED_CHAR,           // Loading the existing character failed
    NO_NETWORK_INFO,            // Network info for player not found
    CREATE_READY_CLIENT,        // Creating of the client with loaded character failed
    RECONNECT_INFO,             // No info for reconnecting character
    RECONNECT_IDENTITY,         // No identity for reconnecting character
    RECONNECT_CHAR,             // No character for reconnecting character
    ALREADY_ON_ANOTHER_SERVER,  // Player is trying to join a server while already being on one
    BIOS_ERROR,                 // Something went wrong while communicating with the BIOS
    
    // RespawnMachine Kicks (0x0050)
    INIT_RESPAWN_IDENTITY = 80, // No identity when initializing respawn
    RESPAWN_IDENTITY,           // No identity when respawning
    NEW_RESPAWN_NETWORK,        // No network info when respawning
    NEW_RESPAWN_IDENTITY,       // No identity when respawning
    NEW_RESPAWN_INFO,           // No info when respawning
    RESPAWN_NEW_CLIENT,         // Creating of client with new respawned character failed
    
    // PlayerVerification Kicks (0x0060)
    PLAYER_VERIFICATION = 96,   // Generic PlayerVerification error (fallback)
    NO_INFO,                    // No player info
    BROKEN_DATA,                // Data could not be verified because it is corrupt
 
    // VerificationError (0x0070)
    VERIFICATION_ERROR = 112,   // Generic Verification error (fallback)
    VE_UNKNOWN,                 // Unknown/unexpected error
    VE_DECOMPRESS,              // Data received by Server is corrupted
    VE_MISSING_MOD,             // Client is missing a mod which is on Server
    VE_EXTRA_MOD,               // Client has a mod which is not on Server
    VE_CLIENT_CORRUPT,          // Client installation is corrupt
    VE_SERVER_CORRUPT,          // Server installation is corrupt
    VE_UNEXPECTED_MOD_PBO,      // Client has a PBO which is not part of Server
    VE_UM_ROGUE_PBO,            // Client has a PBO which is not part of Server
    VE_UM_CLIENT_UPDATED,       // Extra PBO most likely because Client has a newer version of mod
    VE_UM_SERVER_UPDATED,       // Extra PBO most likely because Server has a newer version of mod
    VE_UNEXPECTED_SOURCE,       // The source of the Extra PBO could not be determined
    VE_PATCHED_PBO,             // Client has a patched PBO
    VE_INTEGRITY,               // Client has a corrupted file
    VE_MISSING_BISIGN,          // Client has a PBO which is missing a .bisign
    VE_MISSING_PBO,             // Missing PBO
    VE_M_ROGUE_PBO,             // Missing PBO from unknown origins
    VE_M_CLIENT_UPDATED,        // Missing PBO most likely because Client has a newer version of mod
    VE_M_SERVER_UPDATED,        // Missing PBO most likely because Server has a newer version of mod
    VE_M_SERVER_CORRUPT,        // Server installation is corrupt
    VE_M_CLIENT_CORRUPT,        // Client installation is corrupt
    VE_M_UNEXPECTED_SOURCE,     // The source of the Missing PBO could not be determined
    VE_GPROJ,                   // The Client and Server .gproj are not equal
    
    // PBO Mismatch (0x0090)
    PBO_MISMATCH = 144,         // Server and Client are using different PBO
    PBO_VERSION_MISMATCH,       // Server and Client are using different PBO versions
    PBO_CLIENT_UPDATED,         // Client has a more recent version
    PBO_SERVER_UPDATED,         // Server has a more recent version
    
    // AuthError Kicks (0x00A0)
    AUTH_ERROR = 160,                       // Generic Auth error (fallback)
    AUTH_NO_USER,                           // The user was not found
    
    // Steam Validate (0x00B0)
    AUTH_INVALID_STEAM_VALIDATE = 176,      // Unknown error (API update needed?)
    AUTH_NO_SERVER,                         // Server not initialized properly
    AUTH_INVALID_TICKET,                    // Ticket is not valid.
    AUTH_DUPLICATE_REQUEST,                 // A ticket has already been submitted for this steamID
    AUTH_INVALID_VERSION,                   // Ticket is from an incompatible interface version
    AUTH_GAME_MISMATCH,                     // Ticket is not for this game
    AUTH_EXPIRED_TICKET,                    // Ticket has expired
    
    // Steam OnValidate (0x00C0)
    AUTH_INVALID_STEAM_ONVALIDATE = 192,    // Unknown error (API update needed?)
    AUTH_USER_NOT_CONNECTED,                // The user in question is not connected to steam
    AUTH_LICENCE,                           // The license has expired.
    AUTH_SERVICE_BAN,                       // The user is VAC banned for this game.
    AUTH_LOGGED_IN_ELSEWHERE,               // The user account has logged in elsewhere.
    AUTH_TIMEOUT,                           // VAC has been unable to perform anti-cheat checks on this user
    AUTH_CANCELED,                          // The ticket has been canceled by the issuer
    AUTH_ALREADY_USED,                      // This ticket has already been used, it is not valid.
    AUTH_TICKET_INVALID,                    // This ticket is not from a user instance currently connected to steam.
    AUTH_PUBLISHER_BAN,                     // The user is banned for this game.
    
    // Bios Validate (0x00D0)
    AUTH_INVALID_BIOS_VALIDATE = 208,       // Unknown error (API update needed?)
    AUTH_BIOS_REQUEST_CREATE,               // Failed to create the request to identify user.
    AUTH_BIOS_REQUEST_SEND,                 // Failed to send request
    AUTH_BIOS_REQUEST_WAIT,                 // Failed to receive response
    AUTH_UNKNOWN_BIOS_VALIDATE,             // Unknown error
    
    // Bios OnValidate (0x00E0)
    AUTH_INVALID_BIOS_ONVALIDATE = 224,     // Unknown error (API update needed?)
    AUTH_BIOS_WRONG_RESPONSE,               // Response has unexpected format
    AUTH_UNKNOWN_BIOS_ONVALIDATE,           // Unknown error
 
    // BattlEye Kicks (0x00F0)
    BATTLEYE = 240,             // Kick administred by BattlEye (Can also be Admin Kick)
    
    // Unauthorized Network Message (0x0100)
    UNM1 = 256,
    UNM2,
    UNM3,
    UNM4,
    UNM5,
    UNM6,
