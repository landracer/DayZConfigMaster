// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementthrowing.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZPlayerImplementThrowing
{
    void DayZPlayerImplementThrowing(DayZPlayer pPlayer)
    {
        m_Player = pPlayer;
        SetThrowingModeEnabled(false);
        
        ResetState();
