// Source: F:/Games/Dayz/scripts/3_game/aibehaviour.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
/*
class AIMessage: Managed
 
{
    Param m_Parameters;
    int m_CrcMessage;
    Param GetParameters();
    int GetTypeNameCRC();
}
 
class AIMessageTest : AIMessage
{
    int GetTypeNameCRC() {return testCRC;}
    //static int testCRC = 5;   
}
 
*/
 
 
class AIBehaviourHLData
{
    private void AIBehaviourHLData() {}
    private void ~AIBehaviourHLData() {}
    
    //void ParseConfig(ParamEntryPar param, AIWorld* world, AIAgentTemplate* agentTemplate) {};
    void OnParseConfig();   
    
    //Can be use only inside OnParseConfig function
    proto native void ParseBehaviourSlot(string name);
    proto native void ParseAlertLevel(string name);
    proto native float ReadParamValue(string paramName, float defValue);
    
