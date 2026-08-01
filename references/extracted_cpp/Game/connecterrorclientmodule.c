// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/connecterrorclientmodule.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EConnectErrorClient
{
    UNKNOWN = -1,               // -1 must always be UNKNOWN
    OK = 0,                     // 0 must always be OK
 
    SERVER_UNREACHABLE,         // Could not connect to server because it is not reachable (offline, restarting, ..)
    ALREADY_CONNECTING,         // Client is already attempting to join a server
    ERROR_MSG_SHOWED,           // Indicates there is an error on screen that has yet to be confirmed
    INCORRECT_CLIENT_STATE,     // The client is already connected or is connecting to a server
    INVALID_SESSION,            // The guid of the session is empty
 
    VERSION_MISMATCH,           // Mismatch between server and client version
    VERSION_MISMATCH_RB,        // Client build is lower than &quot;requiredBuild&quot; (server config)
    VERSION_MISMATCH_AB,        // Client build is higher than &quot;allowedBuild&quot; (server config)
 
    DLC_CHECK_FAILED,           // Client does not have required DLC
    EMPTY_PASSWORD,             // Player input an empty password
    
    PASSWORD,                   // Server is password protected
    BE_LICENCE,                 // Server is using BE and it has not yet been agreed to
 
    ALREADY_ON_ANOTHER_SERVER,  // Player is already playing on a different server  
    COMMUNICATION_TIMED_OUT,    // Communication timed out
    ALREADY_ON_SERVER,          // Player is already playing on this server
