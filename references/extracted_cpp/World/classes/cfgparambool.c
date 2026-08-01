// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparambool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgParamBool extends CfgParamType
{   
    private bool m_Value;
    
    void CfgParamBool(string param_name)
    {
        m_Value = false;
