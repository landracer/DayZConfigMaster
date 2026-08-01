// Source: F:/Games/Dayz/scripts/4_world/classes/confighandler/cfgparam.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int CFG_TYPE_STRING   = 0;
const int CFG_TYPE_INT      = 1;
const int CFG_TYPE_FLOAT    = 2;
const int CFG_TYPE_BOOL     = 3;
const int CFG_TYPE_ARRAY    = 4;
const int CFG_TYPE_PARAM    = 5;
 
class CfgParam: Managed
{
    protected string    m_ParamName;
    protected string    m_ParamValueString;
    
    void CfgParam(string param_name)
    {
        m_ParamName = param_name;
