// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/poweredoptic_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PoweredOptic_Base extends ItemOptics
{
    protected PlayerBase                m_Player;
    protected bool                      m_IsActionActive; // sanity check for comparing to EM synched value
    
    void SetPlayer( PlayerBase player )
    {
        m_Player = player;
