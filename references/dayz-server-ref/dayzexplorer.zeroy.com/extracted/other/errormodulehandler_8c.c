// ======================================================================
// File: errormodulehandler_8c_source.html
// Category: other
// ======================================================================

enum ErrorCategory
{
Unknown/* = -1*/,
Generic,
ConnectErrorClient,
ConnectErrorServer,
ConnectErrorScript,
ClientKicked,
BIOSError,
};
class ErrorModuleHandler
{
static proto int ThrowError(ErrorCategory category, int code, string additionalInfo = "");
static proto int ThrowErrorCode(int errorCode, string additionalInfo = "");
static proto int CreateError(ErrorCategory category, int code);
static proto ErrorCategory GetCategoryFromError(int errorCode);
static proto int GetCodeFromError(int errorCode);
static proto string GetErrorHex(int errorCode);
static proto bool AddModule(ErrorCategory category, notnull ErrorHandlerModule errorModule);
static proto bool RemoveModule(ErrorCategory category);
static proto string GetClientMessage(ErrorCategory category, int code, string additionalInfo = "");
static proto string GetClientMessageByCode(int errorCode, string additionalInfo = "");
static proto string GetLastClientMessage(ErrorCategory category, int code);
static proto string GetLastClientMessageByCode(int errorCode);
static proto string GetServerMessage(ErrorCategory category, int code, string additionalInfo = "");
static proto string GetServerMessageByCode(int errorCode, string additionalInfo = "");
static proto string GetLastServerMessage(ErrorCategory category, int code);
static proto string GetLastServerMessageByCode(int errorCode);
static proto native ErrorModuleHandler GetInstance();
static proto void GetErrorModules(notnull out array<ErrorHandlerModule> errorModules);
void SafeAddModule(notnull ErrorHandlerModule errorModule)
{
if ( !AddModule(errorModule.GetCategory(), errorModule) )
Error(string.Format("[EMH] Adding %1 failed. (Category: %2)", errorModule, errorModule.GetCategory()));
}
void SafeRemoveModule(notnull ErrorHandlerModule errorModule)
{
if ( !RemoveModule(errorModule.GetCategory()) )
Error(string.Format("[EMH] Removing %1 failed. (Category: %2)", errorModule, errorModule.GetCategory()));
}
void SafeRemoveModule(ErrorCategory category)
{
if ( !RemoveModule(category) )
Error(string.Format("[EMH] Removing %1 failed.", category));
}
private void Init()
{
if (!g_Game.IsDedicatedServer())
{
SafeAddModule(new ConnectErrorClientModule);
SafeAddModule(new ConnectErrorServerModule);
SafeAddModule(new ConnectErrorScriptModule);
}
SafeAddModule(new ClientKickedModule);
SafeAddModule(new BIOSErrorModule);
}
void OnEvent(EventType eventTypeId, Param params)
{
array<ErrorHandlerModule> errorModules = new array<ErrorHandlerModule>;
GetErrorModules(errorModules);
foreach (ErrorHandlerModule module : errorModules)
{
module.OnEvent(eventTypeId, params);
}
}
}