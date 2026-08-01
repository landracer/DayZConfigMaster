// Source: F:/Games/Dayz/scripts/4_world/classes/corpsedata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CorpseData
{
    const int   GET_LIFETIME_TRIES_MAX = 3;
    
    bool        m_bUpdate;
    int         m_iLastUpdateTime;
    int         m_iTimeOfDeath;
    int         m_iMaxLifetime;
    int         m_iCorpseState;
    int         m_iTriesToGetLifetime;
    
    float       m_LifetimeAdjusted = float.MIN;
    float       m_LastLifetime = float.MIN;
    PlayerBase  m_Player;
    
    void CorpseData(notnull PlayerBase player,int time_of_death)
    {
        m_bUpdate = true;
        m_iLastUpdateTime = time_of_death;
        m_iTimeOfDeath = time_of_death;
        m_iMaxLifetime = -1;
        m_iCorpseState = PlayerConstants.CORPSE_STATE_FRESH;
        m_Player = player;
        m_iTriesToGetLifetime = 0;
