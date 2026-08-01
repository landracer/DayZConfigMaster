// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/chemicalagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChemicalAgent extends AgentBase
{
    override void Init()
    {
        m_Type                  = eAgents.CHEMICAL_POISON;
        m_Invasibility          = 0;
        m_TransferabilityIn     = 0.1;
        m_TransferabilityOut    = 1;
        m_TransferabilityAirOut = 1;
        m_AntibioticsResistance = 1;
        m_MaxCount              = 500;
        m_Potency               = EStatLevels.CRITICAL;
        m_DieOffSpeed           = 0.1;
