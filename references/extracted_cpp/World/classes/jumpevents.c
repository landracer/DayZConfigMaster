// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/jumpevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class JumpEventsBase extends PlayerSoundEventBase
{
    override bool HasPriorityOverCurrent(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type_other)
    {
        return true;
