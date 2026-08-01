// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparamstring.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgParamString extends CfgParamType
{   
    private string m_Value;
    
    void CfgParamString(string param_name)
    {
        m_Value = STRING_EMPTY;
