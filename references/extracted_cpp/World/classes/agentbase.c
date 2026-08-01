// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/agentbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AgentBase : MessageReceiverBase
{
    float   m_Type = 0;
    float   m_Invasibility;                 
    float   m_TransferabilityIn;            
    float   m_TransferabilityOut;           
    float   m_Digestibility = 0.1;          
    int     m_MaxCount = 1;
    int     m_AutoinfectCount = 1;          
    float   m_AutoinfectProbability = CalculateAutoinfectProbability(0);    // [0..1], 0 = zero chance, 1 = 100% chance of getting this agent once per hour
    float   m_TransferabilityAirOut;        
    float   m_AntibioticsResistance = 1;    
    
    protected ref   map<EMedicalDrugsType, float> m_DrugResistances = new map<EMedicalDrugsType, float>();
    
    EStatLevels m_Potency = EStatLevels.MEDIUM;     
    float m_DieOffSpeed = 1;                        
    
    void AgentBase()
    {
        InitDefaults();
        Init();
