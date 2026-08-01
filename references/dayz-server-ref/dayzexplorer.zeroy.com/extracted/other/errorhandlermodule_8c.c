// ======================================================================
// File: errorhandlermodule_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
// Definition
//-----------------------------------------------------------------------------
class ErrorHandlerModule
{
proto native ErrorCategory GetCategory();
proto native void SetCategory(ErrorCategory category);
protected void OnErrorThrown(int errorCode, owned string additionalInfo = "")
{
#ifdef ENABLE_LOGGING
Print(ErrorModuleHandler.GetErrorHex(errorCode));
#endif
}
string GetClientMessage(int errorCode, string additionalInfo = "")
{
return GetSimpleMessage(errorCode, additionalInfo);
}
string GetLastClientMessage(int errorCode)
{
return GetSimpleMessage(errorCode);
}
string GetServerMessage(int errorCode, string additionalInfo = "")
{
return GetSimpleMessage(errorCode, additionalInfo);
}
string GetLastServerMessage(int errorCode)
{
return GetSimpleMessage(errorCode);
}
string GetSimpleMessage(int errorCode, string additionalInfo = "")
{
return string.Format("[%1]: %2", ErrorModuleHandler.GetErrorHex(errorCode), additionalInfo);
}
void OnEvent(EventType eventTypeId, Param params)
{
}
}
//-----------------------------------------------------------------------------
// Script override
//-----------------------------------------------------------------------------
class ErrorHandlerModuleScript : ErrorHandlerModule
{
protected string m_Header = "";
protected string m_Prefix = "";
protected ref UIScriptedMenu m_UIHandler = null;
protected int m_LastErrorThrown = 0;
protected string m_LastAdditionalInfo = "";
protected ref map<int, ref ErrorProperties> m_ErrorDataMap = new map<int, ref ErrorProperties>();
void ErrorHandlerModuleScript()
{
InitOptionalVariables();
FillErrorDataMap();
}
void ~ErrorHandlerModuleScript()
{
if (m_UIHandler)
{
delete m_UIHandler;
}
}
void InitOptionalVariables()
{
}
void FillErrorDataMap()
{
InsertDialogueErrorProperties(-1, "#server_browser_error_unknown");
}
ErrorProperties GetProperties(int errorCode)
{
int error = ErrorModuleHandler.GetCodeFromError(errorCode);
ErrorProperties properties = null;
if (!m_ErrorDataMap.Find(error, properties))
{
Error(string.Format("[EHM] Could not find any properties for error %1(%2) in %3", errorCode, ErrorModuleHandler.GetErrorHex(errorCode), this));
}
return properties;
}
override string GetClientMessage(int errorCode, string additionalInfo = "")
{
ErrorProperties properties = GetProperties(errorCode);
if ( properties )
{
return properties.GetClientMessage(additionalInfo);
}
else
{
return additionalInfo;
}
}
override string GetLastClientMessage(int errorCode)
{
if (errorCode == m_LastErrorThrown)
{
return GetClientMessage(errorCode, m_LastAdditionalInfo);
}
else
{
ErrorEx(string.Format("Was unable to get the information on the last error as another has already occurred. (%1 != %2)", ErrorModuleHandler.GetErrorHex(errorCode), ErrorModuleHandler.GetErrorHex(m_LastErrorThrown)));
return GetClientMessage(errorCode);
}
}
override string GetServerMessage(int errorCode, string additionalInfo = "")
{
ErrorProperties properties = GetProperties(errorCode);
if ( properties )
{
return properties.GetServerMessage(additionalInfo);
}
else
{
return additionalInfo;
}
}
override string GetLastServerMessage(int errorCode)
{
if (errorCode == m_LastErrorThrown)
{
return GetServerMessage(errorCode, m_LastAdditionalInfo);
}
else
{
ErrorEx(string.Format("Was unable to get the information on the last error as another has already occurred. (%1 != %2)", ErrorModuleHandler.GetErrorHex(errorCode), ErrorModuleHandler.GetErrorHex(m_LastErrorThrown)), ErrorExSeverity.WARNING);
return GetServerMessage(errorCode);
}
}
protected override void OnErrorThrown(int errorCode, owned string additionalInfo = "")
{
super.OnErrorThrown(errorCode, additionalInfo);
m_LastErrorThrown = errorCode;
m_LastAdditionalInfo = additionalInfo;
ErrorProperties properties = GetProperties(errorCode);
if ( properties )
{
properties.HandleError(errorCode, additionalInfo);
}
else
{
ErrorEx(string.Format("Error code %1(%2) was thrown but no ErrorProperties was found for it in category %3.", errorCode, ErrorModuleHandler.GetErrorHex(errorCode), GetCategory().ToString()));
if (m_ErrorDataMap.Find(-1, properties))
{
properties.HandleError(errorCode, additionalInfo);
}
}
}
//-----------------------------------------------------------------------------
// Insert helpers
//-----------------------------------------------------------------------------
void InsertDialogueErrorProperties(int code, string message, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2", m_Prefix, message), message, m_Header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertHeaderDialogueErrorProperties(int code, string message, string header, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2", m_Prefix, message), message, header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertPrefixDialogueErrorProperties(int code, string message, string prefix, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2", prefix, message), message, m_Header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertExtendedPrefixDialogueErrorProperties(int code, string message, string prefix, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2%3", m_Prefix, prefix, message), message, m_Header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertExtendedPrefixSplitDialogueErrorProperties(int code, string message, string prefix, string serverMessage, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2%3", m_Prefix, prefix, message), serverMessage, m_Header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertSplitDialogueErrorProperties(int code, string message, string serverMessage, int dialogButtonType = DBT_OK, int defaultButton = DBB_OK, int dialogMeaningType = DMT_EXCLAMATION, bool displayAdditionalInfo = true)
{
m_ErrorDataMap.Insert(code, DialogueErrorProperties(string.Format("%1%2", m_Prefix, message), serverMessage, m_Header, m_UIHandler, dialogButtonType, defaultButton, dialogMeaningType, displayAdditionalInfo));
}
void InsertErrorProperties(int code, string message = "")
{
m_ErrorDataMap.Insert(code, ErrorProperties(message, message));
}
}