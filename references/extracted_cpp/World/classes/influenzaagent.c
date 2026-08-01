// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/agents/influenzaagent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InfluenzaAgent : AgentBase
{
    const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_HC = PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING;
    
    protected const float INVASIBILITY_INC_LOW_HC       = 0.40;
    protected const float INVASIBILITY_INC_MEDIUM_HC    = 0.60;
    protected const float INVASIBILITY_INC_CRITICAL_HC  = 0.70;
    
    protected const float INVASIBILITY_DEC_COMMON_COLD  = -0.3;
    protected const float INVASIBILITY_DEC_INFLUENZA    = -0.3;
    protected const float INVASIBILITY_DEC_PNEUMONIA    = 0.00;
 
    protected const int MAX_TIME_TO_CONTRACT = 8650;            
    protected const int CONTRACT_TIME_THRESHOLD_MIN = 10;       
    protected const int CONTRACT_TIME_THRESHOLD_MAX = 200;      
    
    protected int m_TimeToContract = MAX_TIME_TO_CONTRACT;
 
    protected const float CONTRACT_HEATCOMFORT_MODIFIER[3] = {
        0.25,   // low
        0.6,    // medium
        1.0,    // critical
