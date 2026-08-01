// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/soundevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SoundEventBase
{
    AbstractWave    m_SoundSetCallback;
    int             m_Type;
    int             m_ID;
    int             m_SoundVoiceAnimEventClassID;
    bool            m_RequestDestroy;
    string          m_SoundSetNameRoot;
    bool            m_SkipForControlledCharacter;
    int             m_Param;
    
    
    void ~SoundEventBase()
    {
        if(m_SoundSetCallback) m_SoundSetCallback.Stop();
