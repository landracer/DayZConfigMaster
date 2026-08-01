// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementswimming.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class DayZPlayerImplementSwimming
{
 
    DayZPlayer                  m_pPlayer;
    SHumanCommandSwimSettings   m_pSettings;
    bool                        m_bWasSwimming = false; // important for shallow water, so we know if we should get back into swimming when other fullbody command (like damage) finished
 
    void DayZPlayerImplementSwimming(DayZPlayer pPlayer)
    {
        m_pPlayer   = pPlayer;
        m_pSettings = pPlayer.GetDayZPlayerType().CommandSwimSettingsW();
