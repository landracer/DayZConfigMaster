// ======================================================================
// File: bioserrormodule_8c_source.html
// Category: other
// ======================================================================

enum EBiosError
{
OK,
CANCEL
BAD_PARAMETER,
NOT_FOUND,
NOT_IMPLEMENTED,
PURCHASE_REQUIRED,
NOT_ALLOWED,
BANNED,
LOGICAL,
WRONG_RESPONSE_DATA,
SERVER_HEARTBEAT_SERVER_NOT_FOUND,
SERVER_REGISTER_ALREADY_EXISTS,
REQUEST_CREATE_FAIL_XHR,
REQUEST_SEND_FAIL,
REQUEST_WAIT_FAIL,
NON_JSON_RESPONSE,
// RESERVED SPACE OF REDUNDANT ERRORS
UPDATE_REQUIRED,
UPDATE_REQUIRED_AND_DOWNLOADED,
COMUNICATION_ERROR,
UNKNOWN,
BAD_SCRIPT,
COMMUNICATION_TIMED_OUT,
COMMUNICATION_RESET,
COMMUNICATION_ABORTED
};
class BIOSErrorModule : ErrorHandlerModuleScript
{
void BIOSErrorModule()
{
SetCategory(ErrorCategory.BIOSError);
}
override void InitOptionalVariables()
{
super.InitOptionalVariables();
m_Header = "#STR_BIOS_BIOSError";
}
override void FillErrorDataMap()
{
super.FillErrorDataMap();
InsertErrorProperties(EBiosError.OK); // Not an error, so no message
InsertBIOSError(EBiosError.CANCEL , "#STR_BIOS_OperationCancelled");
InsertBIOSError(EBiosError.BAD_PARAMETER , "#STR_BIOS_BadParameter");
InsertBIOSError(EBiosError.NOT_FOUND , "#STR_BIOS_NotFound");
InsertBIOSError(EBiosError.NOT_IMPLEMENTED , "#STR_BIOS_NotImplemented");
InsertBIOSError(EBiosError.PURCHASE_REQUIRED , "#STR_BIOS_PurchaseRequired");
InsertBIOSError(EBiosError.NOT_ALLOWED , "#STR_BIOS_ActionRestricted");
InsertBIOSError(EBiosError.BANNED , "#STR_auth_publisher_ban");
InsertBIOSError(EBiosError.LOGICAL , "#STR_BIOS_APILogicError");
InsertBIOSError(EBiosError.WRONG_RESPONSE_DATA , "#STR_BIOS_UnexpectedFormat");
InsertBIOSError(EBiosError.SERVER_HEARTBEAT_SERVER_NOT_FOUND , "#STR_BIOS_ServerNotRegistered");
InsertBIOSError(EBiosError.SERVER_REGISTER_ALREADY_EXISTS , "#STR_BIOS_ServerAlreadyRegistered");
InsertBIOSError(EBiosError.REQUEST_CREATE_FAIL_XHR , "#STR_BIOS_FailedToOpenRequest");
InsertBIOSError(EBiosError.REQUEST_SEND_FAIL , "#STR_BIOS_FailedToSendData");
InsertBIOSError(EBiosError.REQUEST_WAIT_FAIL , "#STR_BIOS_FailedToWaitOperationEnd");
InsertBIOSError(EBiosError.NON_JSON_RESPONSE , "#STR_BIOS_UnexpectedFormat");
InsertBIOSError(EBiosError.UPDATE_REQUIRED , "#STR_BIOS_UpdateRequired");
InsertBIOSError(EBiosError.UPDATE_REQUIRED_AND_DOWNLOADED , "#STR_BIOS_UpdateRequiredAndDownloaded");
InsertBIOSError(EBiosError.COMUNICATION_ERROR , "#STR_BIOS_CommunicationError");
InsertBIOSError(EBiosError.UNKNOWN , "#server_browser_error_unknown");
InsertBIOSError(EBiosError.BAD_SCRIPT , "#STR_BIOS_LinkageError");
InsertBIOSError(EBiosError.COMMUNICATION_TIMED_OUT , "#STR_BIOS_CommTimeOutError");
InsertBIOSError(EBiosError.COMMUNICATION_RESET , "#STR_BIOS_CommResetError");
InsertBIOSError(EBiosError.COMMUNICATION_ABORTED , "#STR_BIOS_CommAbortError");
}
void InsertBIOSError(int code, string message)
{
#ifdef PLATFORM_CONSOLE
InsertDialogueErrorProperties(code, message);
#else
InsertErrorProperties(code, message);
#endif
}
}