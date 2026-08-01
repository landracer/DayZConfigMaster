// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/injuryevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InjurySoundEvents extends PlayerSoundEventBase
{
    
    void InjurySoundEvents()
    {
        m_HasPriorityOverTypes = -1;//-1 for all
        m_Type = EPlayerSoundEventType.INJURY;
