// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/debug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

void wpnPrint (string s)
{
#ifdef WPN_DEBUG
    PrintToRPT(&quot;&quot; + s); // comment/uncomment to hide/see at least important info
#else
    //Print(&quot;&quot; + s); // comment/uncomment to hide/see at least important info
#endif
