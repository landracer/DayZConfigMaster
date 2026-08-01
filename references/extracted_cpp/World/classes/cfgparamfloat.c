// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparamfloat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgParamFloat extends CfgParamType
{   
    private float m_Value;
    
    void CfgParamFloat(string param_name)
    {
        m_Value = 0;
