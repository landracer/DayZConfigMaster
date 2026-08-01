// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/staminaevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StaminaSoundEventBase extends PlayerSoundEventBase
{
    const int MAX_VOLUME = 4;
    void StaminaSoundEventBase()
    {
        m_Type = EPlayerSoundEventType.STAMINA;
        m_HasPriorityOverTypes = EPlayerSoundEventType.STAMINA;
