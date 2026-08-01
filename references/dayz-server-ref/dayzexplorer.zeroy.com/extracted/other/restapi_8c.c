// ======================================================================
// File: restapi_8c_source.html
// Category: other
// ======================================================================

// -------------------------------------------------------------------------
// states, (result + error) codes
// defined in C++
enum ERestResultState
{
EREST_EMPTY, // not initialized
EREST_PENDING, // awaiting processing
EREST_FEEDING, // awaiting incoming data
EREST_SUCCESS, // result and/ or data are ready (success), awaiting data processing to be finished (no longer blocking queue processing)
EREST_PROCESSED, // finished (either successfully or with failure) and eill be removed ASAP
EREST_ERROR, // (state >= EREST_ERROR) == error happened
EREST_ERROR_CLIENTERROR, // (EREST_ERROR == EREST_ERROR_CLIENTERROR)
EREST_ERROR_SERVERERROR,
EREST_ERROR_APPERROR,
EREST_ERROR_TIMEOUT,
EREST_ERROR_NOTIMPLEMENTED,
EREST_ERROR_UNKNOWN,
};
// -------------------------------------------------------------------------
// options
// defined in C++
enum ERestOption
{
ERESTOPTION_UNKNOWN, // invalid option
ERESTOPTION_READOPERATION, // read operation timeout (default 10sec)
ERESTOPTION_CONNECTION, // connection timeout (default 10sec)
// note: limit for timeout is between <3 .. 120> seconds, you cannot exceed this value
};
// -------------------------------------------------------------------------
// object to be used from script for result binding
//
// [Example:]
//
// RestCallback cbx1 = new RestCallback;
// RestContext ctx = GetRestApi().GetRestContext("http://somethingsomewhere.com/path/");
// ctx.GET(cbx1,"RequestPath?Argument=Something");
//
// Event are then called upon RestCallback()
//
class RestCallback : Managed
{
void OnError( int errorCode )
{
// override this with your implementation
Print(" !!! OnError() ");
};
void OnTimeout()
{
// override this with your implementation
Print(" !!! OnTimeout() ");
};
void OnSuccess( string data, int dataSize )
{
// override this with your implementation
Print(" !!! OnSuccess() size=" + dataSize );
if( dataSize > 0 )
Print(data); // !!! NOTE: Print() will not output string longer than 1024b, check your dataSize !!!
};
void OnFileCreated( string fileName, int dataSize )
{
// override this with your implementation
Print(" !!! OnFileCreated() file=" + fileName + " size=" + dataSize );
};
};
// -------------------------------------------------------------------------
// context API and request API
class RestContext
{
private void RestContext() {}
private void ~RestContext() {}
proto native int GET( RestCallback cb, string request );
proto native string GET_now( string request );
proto native int FILE( RestCallback cb, string request, string filename );
proto native int FILE_now( string request, string filename );
proto native int POST( RestCallback cb, string request, string data );
proto native string POST_now( string request, string data );
proto native void reset();
proto native void SetHeader( string value );
};
// -------------------------------------------------------------------------
// RestApi core for context create/ access + debug features
class RestApi
{
private void RestApi() {}
private void ~RestApi() {}
proto native RestContext GetRestContext( string serverURL );
proto native int GetContextCount();
proto native void EnableDebug( bool bEnable );
proto native void DebugList();
proto native void SetOption( int option, int value );
};
// -------------------------------------------------------------------------
// RestApi create/ access methods out of Hive initialization
proto native RestApi CreateRestApi();
proto native void DestroyRestApi();
proto native RestApi GetRestApi();