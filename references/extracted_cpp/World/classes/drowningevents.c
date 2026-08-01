// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/drowningevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DrowningSoundEventBase  extends PlayerSoundEventBase
{
    void DrowningEvents()
    {
        m_HasPriorityOverTypes = -1;//-1 for all
        m_Type = EPlayerSoundEventType.DROWNING;
