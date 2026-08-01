// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/woundagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoundAgent : AgentBase
{
    static const float RESISTANCE_STAGE_1 = 1;
    static const float RESISTANCE_STAGE_2 = 0.5;
    
    override void Init()
    {
        m_Type                  = eAgents.WOUND_AGENT;
        m_Invasibility          = 0.208;//to reach 250 in 20 mins
        m_TransferabilityIn     = 1;
        m_TransferabilityOut    = 0;
        m_AntibioticsResistance = 0.5; //deprecated, use m_DrugResistances to initialize this agents resistance to a specific drug, as seen on lines 16
        m_MaxCount              = 500;
        m_Potency               = EStatLevels.GREAT;
        m_DieOffSpeed           = 1;
        m_DrugResistances.Set(EMedicalDrugsType.ANTIBIOTICS, 0.5);  
