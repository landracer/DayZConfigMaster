// Source: F:/Games/Dayz/scripts/3_game/http/restapi.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
 
// -------------------------------------------------------------------------
// states, (result + error) codes
// defined in C++
enum ERestResultState
{
    EREST_EMPTY,                // not initialized
    EREST_PENDING,              // awaiting processing
    EREST_FEEDING,              // awaiting incoming data
    EREST_SUCCESS,              // result and/ or data are ready (success), awaiting data processing to be finished (no longer blocking queue processing)
    EREST_PROCESSED,            // finished (either successfully or with failure) and eill be removed ASAP
 
    EREST_ERROR,                // (state >= EREST_ERROR) == error happened
    EREST_ERROR_CLIENTERROR,    //  (EREST_ERROR == EREST_ERROR_CLIENTERROR)
    EREST_ERROR_SERVERERROR,
    EREST_ERROR_APPERROR,
    EREST_ERROR_TIMEOUT,
    EREST_ERROR_NOTIMPLEMENTED,
    EREST_ERROR_UNKNOWN,
