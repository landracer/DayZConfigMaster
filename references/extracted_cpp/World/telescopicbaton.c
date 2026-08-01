// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/telescopicbaton.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TelescopicBaton : ItemBase
{
    protected int m_MeleeMode;
    protected int m_MeleeHeavyMode;
    protected int m_MeleeSprintMode;
    
    protected ref OpenableBehaviour m_Openable;
    
    void TelescopicBaton()
    {
        m_MeleeMode = 0;
        m_MeleeHeavyMode = 1;
        m_MeleeSprintMode = 2;
 
        m_Openable = new OpenableBehaviour(false);
        
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
 
        UpdateVisualState();
