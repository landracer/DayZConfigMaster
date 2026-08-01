// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/connecterrorservermodule.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EConnectErrorServer
{
    UNKNOWN = -1,           // -1 must always be UNKNOWN
    OK = 0,                 // 0 must always be OK
 
    WRONG_PASSWORD,         // User entered wrong password
    WRONG_VERSION,          // Server and client do not have the same build while &quot;forceSameBuild&quot; is true
    CONNECT_ERROR,          // Unknown error
    SESSION_FULL,           // Server is full
    FILE_PATCHING,          // Bad file patching
 
    ALREADY_CONNECTED,      // Client is already connected
    HOST_ADDRESS,           // Was unable to resolve host address
    NET_CHANNEL,            // Client failed to create NetChannel
    CONNECT,                // Connecting itself has failed
    REQUEST_PLAYER,         // Failed to request player
    CREATE_PLAYER,          // Failed to create player
    TIMEOUT,                // Packages timed out
    UID,                    // Failed to find uid   
    NET_CHANNEL_SERVER,     // Server failed to create NetChannel   
    ACTIVATE,               // Failed to activate session
    SERVER_LOCKED,          // Server is locked by admin
    SERVER_SHUTDOWN,        // Server is shutting down soon
    BUILD_SMALLER,          // Client has a build smaller than the Server
    BUILD_GREATER,          // Client has a build greater than the Server
    VERSION_SMALLER,        // Client has a version smaller than the Server
    VERSION_GREATER,        // Client has a version greater than the Server
    EQUAL_MOD,              // Server and Client do not have equal mods while &quot;equalModRequired&quot; is true
    CLIENT_DIAG,            // Client is using Diag exe while Server is not
    SERVER_DIAG,            // Server is using Diag exe while Client is not
    NO_DEVICE_ID,           // Client has not acquired device id
