// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/heavymetalagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeavyMetalAgent : AgentBase
{
    override void Init()
    {
        m_Type                  = eAgents.HEAVYMETAL;
        m_TransferabilityIn     = 0.5;
        m_TransferabilityOut    = 0;
        m_AntibioticsResistance = 1; //deprecated, use m_DrugResistances to initialize this agents resistance to a specific drug, as seen on lines 11
        m_MaxCount              = 600;
        m_Potency               = EStatLevels.MEDIUM;
        m_DrugResistances.Set(EMedicalDrugsType.CHELATION, 0.0);
