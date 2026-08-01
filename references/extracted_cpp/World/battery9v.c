// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/battery9v.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Battery9V : ItemBase
{
    private int             m_Efficiency0To10; // Synchronized variable
    static private float    m_EfficiencyDecayStart = 0.1; // At this % of maximum energy the output of the battery starts to weaken.
    
    void Battery9V()
    {
        m_Efficiency0To10 = 10;
        RegisterNetSyncVariableInt(&quot;m_Efficiency0To10&quot;);
