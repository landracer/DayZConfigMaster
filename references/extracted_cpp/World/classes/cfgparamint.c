// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparamint.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgParamInt extends CfgParamType
{   
    private int m_Value;
    
    void CfgParamInt(string param_name)
    {
        m_Value = 0;
