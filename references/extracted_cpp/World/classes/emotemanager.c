// Source: F:/Games/Dayz/scripts/4_world/classes/emotemanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EmoteCB : HumanCommandActionCallback
{
    bool            m_IsFullbody;
    int             m_callbackID; //Animation cmd ID
    PlayerBase      m_player;
    EmoteManager    m_Manager;
    
    void ~EmoteCB()
    {
        if (m_Manager)
        {
            m_Manager.OnCallbackEnd();
        }
        
        if (g_Game && m_player)
            m_player.RequestHandAnimationStateRefresh();
