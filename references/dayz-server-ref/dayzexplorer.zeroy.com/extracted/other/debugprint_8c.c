// ======================================================================
// File: debugprint_8c_source.html
// Category: other
// ======================================================================

class DebugPrint
{
static private const int MSG_LOG = 0;
static private const int MSG_WARNING = 1;
static private const int MSG_ERROR = 2;
static private const int MSG_COUNT = 3;
static private string s_MsgPrefix[MSG_COUNT];
static private string s_MsgStackMarkStart;
static private string s_MsgStackMarkEnd;
static private bool s_MsgStackMarked;
static private bool s_TraceAllLogs;
static void OnInit()
{
s_MsgPrefix[MSG_LOG] = "Log";
s_MsgPrefix[MSG_WARNING] = "Warning";
s_MsgPrefix[MSG_ERROR] = "Error";
s_MsgStackMarkStart = "-- Stack trace --";
s_MsgStackMarked = false;
s_MsgStackMarkEnd = "-----------------";
s_TraceAllLogs = false;
}
static void Log(string msg)
{
LogMessage(msg, MSG_LOG, s_TraceAllLogs);
}
static void LogAndTrace(string msg)
{
LogMessage(msg, MSG_LOG, true);
}
static void LogWarning(string msg)
{
LogMessage(msg, MSG_WARNING, s_TraceAllLogs);
}
static void LogWarningAndTrace(string msg)
{
LogMessage(msg, MSG_WARNING, true);
}
static void LogError(string msg)
{
LogMessage(msg, MSG_ERROR, s_TraceAllLogs);
}
static void LogErrorAndTrace(string msg)
{
LogMessage(msg, MSG_ERROR, true);
}
static string AdjustDebugLog(string msg)
{
if ( IsStackTrace(msg) )
{
return TrimStackTrace(msg);
}
if ( IsDebugLog(msg) )
{
return TrimDebugLog(msg);
}
return msg;
}
static void EnableTracingLogs(bool enable)
{
s_TraceAllLogs = enable;
}
static private bool IsDebugLog(string msg)
{
for ( int i = 0; i < MSG_COUNT; ++i )
{
if ( msg.IndexOf(s_MsgPrefix[i]) != -1 )
{
return true;
}
}
return false;
}
static private string TrimDebugLog(string msg)
{
int msg_lenght = msg.Length();
int log_start = msg.IndexOf("&#39;") + 1;
if ( log_start == -1 )
{
return msg;
}
int log_lenght = msg_lenght - log_start - 2;
return msg.Substring(log_start, log_lenght);
}
static private bool IsStackTrace(string msg)
{
if ( s_MsgStackMarked && msg.IndexOf(s_MsgStackMarkEnd) != -1 )
{
s_MsgStackMarked = false;
return false;
}
if ( s_MsgStackMarked )
{
return true;
}
if ( msg.IndexOf(s_MsgStackMarkStart) != -1 )
{
s_MsgStackMarked = true;
return true;
}
return false;
}
static private string TrimStackTrace(string msg)
{
if ( msg.IndexOf("DebugPrint.c") != -1 )
{
return string.Empty;
}
return msg;
}
static private void LogMessage(string msg, int msg_type, bool trace=false)
{
string mesg = "["+s_MsgPrefix[msg_type]+"]: "+msg;
Print(mesg);
if ( trace )
{
DumpStack();
}
}
};