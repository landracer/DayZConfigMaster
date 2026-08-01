// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementjumpclimb.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZPlayerImplementJumpClimb
{
    bool m_bIsJumpInProgress;   
    
    private DayZPlayerImplement m_Player;
    private bool m_bWasClimb;
 
    void DayZPlayerImplementJumpClimb(DayZPlayerImplement pPlayer)
    {
        m_Player = pPlayer;
