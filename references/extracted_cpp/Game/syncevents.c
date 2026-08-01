// Source: F:/Games/Dayz/scripts/3_game/syncevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SyncEvents
{
    static void RegisterEvents()
    {
        DayZGame dz_game = DayZGame.Cast( g_Game );
        
        dz_game.Event_OnRPC.Insert( Event_OnRPC );
        Print(&quot;SyncEvents -> RegisterEvents&quot;);
