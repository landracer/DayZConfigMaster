// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/debug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

void syncDebugPrint (string s)
{
#ifdef LOG_TO_RPT
    PrintToRPT(&quot;&quot; + s); // comment/uncomment to hide/see debug logs
#endif
    
#ifdef LOG_TO_SCRIPT
    Print(string.Format(&quot;%1&quot;, s));
#endif
