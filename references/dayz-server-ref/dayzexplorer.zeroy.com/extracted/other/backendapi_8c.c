// ======================================================================
// File: backendapi_8c_source.html
// Category: other
// ======================================================================

enum EBackendError
{
EBERR_OK, // all OK
EBERR_UNKNOWN, // unknown error
EBERR_DISABLED, // backend is disabled
EBERR_INVALID_STATE, // called request from state where it is not possible (ie. reading data before logon and such)
EBERR_BUSY, // no request can be called - login/ auth in process
EBERR_ALREADY_OFFLINE, // state is already active
EBERR_ALREADY_ONLINE, // state is already active
EBERR_ALREADY_REQUESTED, // state already requested once!
EBERR_LOGIN_FAILED, // failed to logon
EBERR_AUTH_FAILED, // failed to authenticate
EBERR_LOGIN_SUCCESS, // logon successfull
EBERR_AUTH_SUCCESS, // authenticate successfull
EBERR_CONFIGURATION_GET, // configuration received
EBERR_CEPROFILE_GET, // CE profile configuration received
EBERR_CHARACTER_GET, // character data receieved
EBERR_CHARACTER_UPDATE, // character update done
};
enum EBackendRequest
{
/* // game api
EBREQ_GAME_Test, // environment test - dummy data read
EBREQ_GAME_World, // static world configuration read
EBREQ_GAME_CEProfile,
EBREQ_GAME_CharacterGet, // character data read
EBREQ_GAME_CharacterUpdate, // character data update
// #if BACKENDAPI_DEV_CHARACTER
EBREQ_GAME_DevCharacterGet, // dev character data read
EBREQ_GAME_DevCharacterUpdate, // dev character data update
// #endif
EBREQ_GAME_Heartbeat,*/
// user api request <> response
EBREQ_USER_Login,
EBREQ_USER_Auth,
/* // lobby api request <> response
EBREQ_LOBBY_RoomsRegister,
EBREQ_LOBBY_RoomsJoin,
EBREQ_LOBBY_RoomsAcceptPlayer,
EBREQ_LOBBY_RoomsHeartBeat,
EBREQ_LOBBY_RoomsUpdate,
EBREQ_LOBBY_RoomsRemovePlayer,
EBREQ_LOBBY_RoomsSearch,
EBREQ_LOBBY_RoomsGetByIds,
EBREQ_LOBBY_RoomsGetByHostIds,
EBREQ_LOBBY_GetActiveScenarios,
// storage api request <> response
EBREQ_STORAGE_GetFileTempURL,
EBREQ_STORAGE_GetFileTempURLS2S,
EBREQ_STORAGE_GetFile,
EBREQ_STORAGE_DeleteFile,
EBREQ_STORAGE_GetFileS2S,
EBREQ_STORAGE_DeleteFileS2S,
EBREQ_STORAGE_PatchFileS2S,
EBREQ_STORAGE_Upload,
EBREQ_STORAGE_UploadS2S,
EBREQ_STORAGE_UploadZip,
EBREQ_STORAGE_UploadZipS2S,
EBREQ_STORAGE_Limits,
EBREQ_STORAGE_LimitsS2S,
// feedback request <> response
// #if ONLINE_SLACKAPI
EBREQ_SLACKAPI_PostMessage,
// #endif*/
};
enum EBackendCredentials
{
EBCRED_NAME,
EBCRED_PWD,
EBCRED_BASEURI,
};
// -------------------------------------------------------------------------
// Callback interface for backend - must exist for the duration of request!
class BackendCallback : Managed
{
void OnDataReceive( string data, int size )
{
Print("[BackendCallback] Data received, size=" + size);
Print(data);
}
void OnError( int code )
{
Print("[BackendCallback] OnError: " + GetBackendApi().GetErrorCode(code));
}
void OnSuccess( int code )
{
Print("[BackendCallback] OnSuccess()");
}
void OnTimeout()
{
Print("[BackendCallback] OnTimeout");
}
};
// -------------------------------------------------------------------------
// Backend API access
class BackendApi
{
private void BackendApi() {}
private void ~BackendApi() {}
proto native bool Initiate();
proto native bool Shutdown();
proto native bool IsDisconnected();
proto native bool IsRuntime();
proto native bool IsBusy();
string GetErrorCode( int code )
{
string result;
if ( code == EBackendError.EBERR_OK )
result = "OK";
else if ( code == EBackendError.EBERR_UNKNOWN )
result = "Offline";
else if ( code == EBackendError.EBERR_DISABLED )
result = "Communication Disabled";
else if ( code == EBackendError.EBERR_INVALID_STATE )
result = "Cannot be called from current state";
else if ( code == EBackendError.EBERR_BUSY )
result = "Busy processing requests";
else if ( code == EBackendError.EBERR_ALREADY_OFFLINE )
result = "Already disconnected";
else if ( code == EBackendError.EBERR_ALREADY_ONLINE )
result = "Already connected";
else if ( code == EBackendError.EBERR_LOGIN_FAILED )
result = "Failed to logon";
else if ( code == EBackendError.EBERR_AUTH_FAILED )
result = "Failed to Authenticate";
else
result = "*";
return result;
}
void OnCannotInitiate( int code )
{
Print("!!! [Backend] Cannot Initiate: "+ GetErrorCode(code));
}
void OnCannotShutdown( int code )
{
Print("!!! [Backend] Cannot Shutdown: "+ GetErrorCode(code));
}
void OnSuccess( string step )
{
Print( "[Backend] Successfully Solicited: " + step );
}
void OnFail( string step )
{
Print( "[Backend] Failed to Proceed: " + step );
}
proto native void Request( int request, BackendCallback cb, JsonApiStruct dataObject );
proto native void PlayerRequest( int request, BackendCallback cb, JsonApiStruct dataObject, int iPlayerId );
proto native void FeedbackMessage( BackendCallback cb, JsonApiStruct dataObject, string message );
proto native void SetCredentialsItem( EBackendCredentials item, string str );
proto native string GetCredentialsItem( EBackendCredentials item );
proto native void VerifyCredentials();
};
proto native BackendApi GetBackendApi();
// -------------------------------------------------------------------------