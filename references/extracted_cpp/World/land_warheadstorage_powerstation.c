// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/land_warheadstorage_powerstation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_WarheadStorage_PowerStation : House
{
    protected bool m_InitBunkerState;   // if generator is running already, used for initial power setup of related bunker
    protected static ref set<Land_WarheadStorage_Main>  m_Bunkers;
    protected ref Timer m_UpdateTimer;
    protected PowerGeneratorStatic m_PowerGenerator;
    protected bool m_IsPowerGeneratorRunning;
    protected int m_PlaySparks;
    protected int m_LastActivatedLeverId;
    protected int m_LeverStatesBits;    // is persistent through power generator
    
    protected static ref map<Land_WarheadStorage_PowerStation, Land_WarheadStorage_Main> m_BunkerStationMap = new map<Land_WarheadStorage_PowerStation, Land_WarheadStorage_Main>();
    
    protected const string LEVERS_POS_MEMPOINT      = &quot;leverup_axis&quot;;
    protected const string GENERATOR_POS_MEMPOINT   = &quot;generator_pos&quot;;
    
    protected const string SWITCH_UP_SOUND      = &quot;Power_Station_Switch_Up_SoundSet&quot;;
    protected const string SWITCH_DOWN_SOUND    = &quot;Power_Station_Switch_Down_SoundSet&quot;;
    protected const string SPARKLES_SOUND       = &quot;Power_Station_generator_overpowered_SoundSet&quot;;
        
    void Land_WarheadStorage_PowerStation()
    {
        m_PlaySparks = 0;
        
        RegisterNetSyncVariableInt(&quot;m_PlaySparks&quot;, 0, 4);
