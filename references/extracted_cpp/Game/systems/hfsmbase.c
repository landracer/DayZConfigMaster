// Source: F:/Games/Dayz/scripts/3_game/systems/hfsmbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

void fsmDebugPrint (string s)
{
#ifdef FSM_DEBUG
    PrintToRPT(&quot;&quot; + s); // comment/uncomment to hide/see debug logs
#else
    //Print(&quot;&quot; + s); // comment/uncomment to hide/see debug logs
#endif
