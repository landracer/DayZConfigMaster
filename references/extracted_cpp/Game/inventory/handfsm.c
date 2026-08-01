// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handfsm.c Source File
// Extracted from DayZ Code Explorer by Zeroy

void hndDebugPrint (string s)
{
#ifdef INV_DEBUG
    PrintToRPT(&quot;&quot; + s); // comment/uncomment to hide/see debug logs
#else
    //Print(&quot;&quot; + s); // comment/uncomment to hide/see debug logs
#endif
