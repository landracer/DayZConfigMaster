// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/heatcomfortevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeatComfortEventsBase extends PlayerSoundEventBase
{
    
    void HeatComfortEventsBase()
    {
        m_HasPriorityOverTypes = EPlayerSoundEventType.DUMMY | EPlayerSoundEventType.INJURY | EPlayerSoundEventType.STAMINA | EPlayerSoundEventType.GENERAL;//-1 for all
        m_Type = EPlayerSoundEventType.GENERAL;
