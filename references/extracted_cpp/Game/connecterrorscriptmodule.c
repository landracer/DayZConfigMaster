// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/connecterrorscriptmodule.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EConnectErrorScript
{
    UNKNOWN = -1,               // -1 must always be UNKNOWN
    OK = 0,                     // 0 must always be OK
    
    ALREADY_CONNECTING,         // Already joining a server
    ALREADY_CONNECTING_THIS,    // Already joining this exact server
}
 
class ConnectErrorScriptModule : ErrorHandlerModuleScript
{
    void ConnectErrorScriptModule()
    {
        SetCategory(ErrorCategory.ConnectErrorScript);      
