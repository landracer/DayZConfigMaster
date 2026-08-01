// Source: F:/Games/Dayz/scripts/3_game/http/backendapi.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 

    enum EBackendError
    {
        EBERR_OK,                   // all OK
        EBERR_UNKNOWN,              // unknown error
        EBERR_DISABLED,             // backend is disabled
        EBERR_INVALID_STATE,        // called request from state where it is not possible (ie. reading data before logon and such)
        EBERR_BUSY,                 // no request can be called - login/ auth in process
        EBERR_ALREADY_OFFLINE,      // state is already active
        EBERR_ALREADY_ONLINE,       // state is already active
        EBERR_ALREADY_REQUESTED,    // state already requested once!
        EBERR_LOGIN_FAILED,         // failed to logon
        EBERR_AUTH_FAILED,          // failed to authenticate
        EBERR_LOGIN_SUCCESS,        // logon successfull
        EBERR_AUTH_SUCCESS,         // authenticate successfull
        EBERR_CONFIGURATION_GET,    // configuration received
        EBERR_CEPROFILE_GET,        // CE profile configuration received
        EBERR_CHARACTER_GET,        // character data receieved
        EBERR_CHARACTER_UPDATE,     // character update done
