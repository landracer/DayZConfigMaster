// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/errorhandlermodule.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------------------------------------------------------
// Definition
//-----------------------------------------------------------------------------
class ErrorHandlerModule
{
    proto native ErrorCategory GetCategory();
    
    proto native void SetCategory(ErrorCategory category);
    
    protected void OnErrorThrown(int errorCode, owned string additionalInfo = &quot;&quot;)
    {
        #ifdef ENABLE_LOGGING
        Print(ErrorModuleHandler.GetErrorHex(errorCode));
        #endif
