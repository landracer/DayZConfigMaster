// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/cattleprod.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CattleProd : ItemBase
{
    protected int m_MeleeMode;
    protected int m_MeleeHeavyMode;
    protected int m_MeleeSprintMode;
 
    void CattleProd()
    {
        m_MeleeMode = 0;
        m_MeleeHeavyMode = 1;
        m_MeleeSprintMode = 2;
