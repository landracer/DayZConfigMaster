// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/stunbaton.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StunBaton : ItemBase
{
    protected int m_MeleeMode;
    protected int m_MeleeHeavyMode;
    protected int m_MeleeSprintMode;
 
    void StunBaton()
    {
        m_MeleeMode = 0;
        m_MeleeHeavyMode = 1;
        m_MeleeSprintMode = 2;
