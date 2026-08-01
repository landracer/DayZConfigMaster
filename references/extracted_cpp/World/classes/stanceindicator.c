// Source: F:/Games/Dayz/scripts/4_world/classes/stanceindicator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StanceIndicator
{   
    protected ref HumanMovementState    m_State;
    protected PlayerBase                m_Player;
    
    void StanceIndicator(PlayerBase player)
    {
        m_State = new HumanMovementState();
        m_Player = player;
