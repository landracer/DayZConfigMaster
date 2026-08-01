// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/statecb.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SymptomCB extends HumanCommandActionCallback
{
    //int m_SymptomUID;
    float m_RunTime;
    float m_StartingTime;
    PlayerBase m_Player;
    
    override void OnFinish(bool pCanceled)
    {
        if( m_Player && m_Player.GetSymptomManager())
        {
            m_Player.GetSymptomManager().OnAnimationFinished();
        }
