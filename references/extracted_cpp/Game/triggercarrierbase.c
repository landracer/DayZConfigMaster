// Source: F:/Games/Dayz/scripts/3_game/triggercarrierbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UndergroundTriggerCarrierBase : ScriptedEntity
{
    int m_TriggerIndex = -1;
    
    protected int m_ParentObjectNetIdLow    = -1;
    protected int m_ParentObjectNetIdHigh   = -1;
 
    protected Object m_ParentObject;
    
    void SetIndex(int index)
    {
        m_TriggerIndex = index;
        SetSynchDirty();
