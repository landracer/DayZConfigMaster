// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/undergroundtrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class UndergroundTriggerCarrier : UndergroundTriggerCarrierBase
{
    ref JsonUndergroundAreaTriggerData m_Data;
    UndergroundTrigger m_Trigger;
    
    void UndergroundTriggerCarrier()
    {
        RegisterNetSyncVariableInt(&quot;m_TriggerIndex&quot;, -1, 255);
        RegisterNetSyncVariableInt(&quot;m_ParentObjectNetIdLow&quot;);
        RegisterNetSyncVariableInt(&quot;m_ParentObjectNetIdHigh&quot;);
