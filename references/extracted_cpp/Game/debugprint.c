// Source: F:/Games/Dayz/scripts/3_game/tools/debugprint.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DebugPrint
{   
    static private const int    MSG_LOG     = 0;
    static private const int    MSG_WARNING = 1;
    static private const int    MSG_ERROR   = 2;
    static private const int    MSG_COUNT   = 3;
    
    static private string       s_MsgPrefix[MSG_COUNT];
    static private string       s_MsgStackMarkStart;
    static private string       s_MsgStackMarkEnd;
    static private bool         s_MsgStackMarked;
    static private bool         s_TraceAllLogs;
    
    static void OnInit()
    {
        s_MsgPrefix[MSG_LOG]        = &quot;Log&quot;;
        s_MsgPrefix[MSG_WARNING]    = &quot;Warning&quot;;
        s_MsgPrefix[MSG_ERROR]      = &quot;Error&quot;;
        
        s_MsgStackMarkStart         = &quot;-- Stack trace --&quot;;
        s_MsgStackMarked            = false;
        s_MsgStackMarkEnd           = &quot;-----------------&quot;;
        
        s_TraceAllLogs              = false;
