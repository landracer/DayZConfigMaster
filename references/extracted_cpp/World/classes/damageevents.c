// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/damageevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DamageSoundEvents extends PlayerSoundEventBase
{
    
    void DamageSoundEvents()
    {
        m_HasPriorityOverTypes = -1;//-1 for all
        m_Type = EPlayerSoundEventType.DAMAGE;
