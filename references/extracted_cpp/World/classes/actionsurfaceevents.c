// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/replacesoundevents/events/actionsurfaceevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
//Digging
class DigPrimarySoundEvent extends DigSoundEventBase
{
    void DigPrimarySoundEvent()
    {
        m_EventType = ESoundEventType.SOUND_COMMON;
        m_ID = EReplaceSoundEventID.DIG_PRIMARY;
        m_SoundAnimEventClassID = 887;
