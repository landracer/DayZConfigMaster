// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/raycaster.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Item for raycast debugging. It&#39;s also fun to play around with at night.
 
class Raycaster extends ItemBase
{
    protected ref Timer     m_Timer;
    
    override void OnWorkStart()
    {
        if ( !g_Game.IsServer()  ||  !g_Game.IsMultiplayer() ) // Client side
        {
            StartPeriodicMeasurement();
        }
