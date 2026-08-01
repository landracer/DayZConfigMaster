// Source: F:/Games/Dayz/scripts/3_game/analytics/analyticsmanagerclient.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnalyticsManagerClient
{
    static const int GEAR_COUNT = 3; 
    static string m_FullGear[GEAR_COUNT] = {&quot;Shoulder&quot;,&quot;Melee&quot;,&quot;Back&quot;};
    
    void RegisterEvents()
    {
        ClientData.SyncEvent_OnEntityKilled.Insert(Event_OnEntityKilled);
        ClientData.SyncEvent_OnPlayerIgnitedFireplace.Insert(Event_OnPlayerIgnitedFireplace);
