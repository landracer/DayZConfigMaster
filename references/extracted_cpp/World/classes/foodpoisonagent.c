// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/foodpoisonagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FoodPoisonAgent : AgentBase
{
    override void Init()
    {
        m_Type                  = eAgents.FOOD_POISON;
        m_Invasibility          = 1.0;
        m_TransferabilityIn     = 1.0;
        m_TransferabilityOut    = 0.0;
        m_AntibioticsResistance = 1.0; //deprecated, use m_DrugResistances to initialize this agents resistance to a given drug
        m_MaxCount              = 800;
        m_Potency               = EStatLevels.GREAT;
        m_DieOffSpeed           = 0.0;
        m_Digestibility         = 0.5;
 
