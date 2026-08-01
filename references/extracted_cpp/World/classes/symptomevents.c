// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/symptomevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SymptomSoundEventBase extends PlayerSoundEventBase
{
    void SymptomSoundEventBase()
    {
        m_Type = EPlayerSoundEventType.GENERAL;
        m_HasPriorityOverTypes = -1;
