// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/land_warheadstorage_main.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_WarheadStorage_Main : House
{
    protected bool m_HasPowerPrev;
    protected bool m_IsLowEnergy;
    protected WarheadStorageLight m_StorageDoorLights[4];
    protected EffectSound m_SoundDoorLoop[4];
    protected ref array<EffectSound> m_PoweredSoundEffects = new array<EffectSound>();
    protected ref map<int, ref Timer> m_AutoCloseTimers;
    
    protected const float DOOR_AUTOCLOSE_TIME = 12;//how long before the outer door auto-close after being opened
    protected const int SOURCES_COUNT = 4;  // storage lights, side vents, lamps
    
    protected const string WARHEAD_BUNKER_TRIGGER = &quot;UndergroundBunkerTrigger&quot;;
    
    protected const string ALARM_POS_MEMPOINT       = &quot;underground&quot;;
    protected const string VENT_MAIN_POS_MEMPOINT   = &quot;Ventilation_main&quot;;
    protected const string VENT_POS_MEMPOINT        = &quot;Ventilation&quot;;
 
    protected const string ALARM_DOOR_OPEN_SOUND        = &quot;UndergroundDoor_Alarm_Start_SoundSet&quot;;
    protected const string ALARM_DOOR_OPEN_LOOP_SOUND   = &quot;UndergroundDoor_Alarm_Loop_SoundSet&quot;;
    protected const string ALARM_DOOR_CLOSE_SOUND       = &quot;UndergroundDoor_Alarm_End_SoundSet&quot;;
    protected const string VENTILATION_SOUND            = &quot;Bunker_Ventilation_SoundSet&quot;;
    protected const string LAMPS_SOUND                  = &quot;Bunker_Lamp_Hum_SoundSet&quot;;
    protected const string ELECTRICITY_ON_SOUND         = &quot;Bunker_bunker_electricity_on_SoundSet&quot;;
    protected const string ELECTRICITY_OFF_SOUND        = &quot;Bunker_bunker_electricity_off_SoundSet&quot;;
    
    protected const string LAMP_SELECTION           = &quot;GlassLamp&quot;;
    protected const string MAIN_DOOR_SELECTION1     = &quot;maindoor1_outer&quot;;
    protected const string MAIN_DOOR_SELECTION2     = &quot;maindoor2_outer&quot;;
    
    protected const string COLOR_LAMP_OFF   = &quot;DZ\\structures_sakhal\\military\\storage\\data\\Warhead_Storage_Lamp_Glass_int.rvmat&quot;;
    protected const string COLOR_LAMP_ON    = &quot;DZ\\structures_sakhal\\military\\storage\\data\\Warhead_Storage_Lamp_Glass_e_int.rvmat&quot;;
    
    // sync
    protected bool m_HasPower;
    protected bool m_LeverStatesBits;
    
    void Land_WarheadStorage_Main()
    {
        RegisterNetSyncVariableBool(&quot;m_HasPower&quot;);
        RegisterNetSyncVariableInt(&quot;m_LeverStatesBits&quot;);
        RegisterNetSyncVariableInt(&quot;m_IsLowEnergy&quot;);
        
        Land_WarheadStorage_PowerStation.RegisterBunker(this);
