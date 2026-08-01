// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/burlapsackcover.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BurlapSackCover extends HeadGear_Base
{
    protected PlayerBase m_Player;
        
    void ~BurlapSackCover()
    {
        if (m_Player)
        {
            OnRemovedFromHead(m_Player);
        }
