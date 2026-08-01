// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/stairs/land_underground_stairs_exit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Underground_Stairs_Exit : Land_Underground_EntranceBase
{
    EffectSound         m_LockingSound;
    
    EffectSound         m_OpenSoundIn;
    //EffectSound       m_OpenSoundLoop;
    EffectSound         m_OpenSoundOut;
 
    EffectSound         m_CloseSoundIn;
    EffectSound         m_CloseSoundLoop;
    EffectSound         m_CloseSoundOut;
 
    const string LOCKING_SOUNDSET           = &quot;UndergroundDoor_Lock_SoundSet&quot;;
    const string OPENING_SOUNDSET_LOOP_IN   = &quot;UndergroundSmallExitDoor_Open_Start_SoundSet&quot;;
    const string OPENING_SOUNDSET_LOOP      = &quot;UndergroundSmallExitDoor_Open_Loop_SoundSet&quot;;
    const string OPENING_SOUNDSET_LOOP_OUT  = &quot;UndergroundSmallExitDoor_Open_End_SoundSet&quot;;
    
    const string CLOSING_SOUNDSET_LOOP_IN   = &quot;UndergroundSmallExitDoor_Close_Start_SoundSet&quot;;
    const string CLOSING_SOUNDSET_LOOP      = &quot;UndergroundSmallExitDoor_Close_Loop_SoundSet&quot;;
    const string CLOSING_SOUNDSET_LOOP_OUT  = &quot;UndergroundSmallExitDoor_Close_End_SoundSet&quot;;
    
    void Land_Underground_Stairs_Exit()
    {
        m_DoorType = EUndegroundDoorType.SMALL;
