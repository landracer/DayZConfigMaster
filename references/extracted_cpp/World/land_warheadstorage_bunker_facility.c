// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/land_warheadstorage_bunker_facility.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_WarheadStorage_Bunker_Facility : House
{
    protected const float DOOR_AUTOCLOSE_TIME = 12; //how long before the door auto-close after being opened
 
    protected ref map<int, ref Timer> m_AutoCloseTimers;
    
    override void DeferredInit()
    {
        g_Game.RegisterNetworkStaticObject(this);
        
        if (g_Game.IsServer())
        {
            for (int i = 0; i < GetDoorCount(); ++i)
                AutoCloseDoor(i);
 
            JsonUndergroundTriggers.SpawnParentedTriggers(this);
        }
