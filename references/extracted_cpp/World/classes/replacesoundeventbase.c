// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/replacesoundevents/replacesoundeventbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ReplaceSoundEventBase
{
    protected ESoundEventType       m_EventType;
    protected EReplaceSoundEventID  m_ID;
    protected int                   m_SoundAnimEventClassID;
    protected int                   m_ReplacedSoundAnimID;
    protected string                m_UserString;
    protected PlayerBase            m_Player;
    
    ESoundEventType GetSoundEventType()
    {
        return m_EventType;
