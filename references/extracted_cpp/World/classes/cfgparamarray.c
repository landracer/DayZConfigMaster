// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparamarray.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgParamArray extends CfgParam
{
    private ref array<ref CfgParam> m_Values;
    
    void CfgParamArray(string param_name)
    {
        m_Values = new array<ref CfgParam>;
