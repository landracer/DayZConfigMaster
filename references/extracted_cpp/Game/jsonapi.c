// Source: F:/Games/Dayz/scripts/3_game/http/jsonapi.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
// -------------------------------------------------------------------------
// error codes for handle processing
// defined in C++
enum EJsonApiError
{
    ETJSON_UNKNOWN,         // invalid code
 
    ETJSON_OK,              // all fine
    ETJSON_COMMSEND,        // error during send
    ETJSON_COMMRECV,        // error during receive
    ETJSON_PARSERERROR,     // error during parsing
    ETJSON_PACKNOSTART,     // error - cannot start packing (invalid state)
    ETJSON_TIMEOUT,         // failed to send/ store handle due to timeout
    ETJSON_NOBUFFERS,       // not enough buffers available
    ETJSON_FAILFILELOAD,    // failed to load file
    ETJSON_FAILFILESAVE,    // failed to save file
    ETJSON_NOTARRAY,        // object is not array (ie. attempt to provide different or none object as array)
