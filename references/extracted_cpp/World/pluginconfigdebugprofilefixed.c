// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginconfighandler/pluginconfigdebugprofilefixed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginConfigDebugProfileFixed extends PluginConfigDebugProfile
{
    const string POSITIONS_LIST         = &quot;console_positions&quot;;
    const string POSITIONS_LIST_ENOCH   = &quot;console_positions_enoch&quot;;
    const string SUB_PARAM_POS_NAME     = &quot;name&quot;;
    const string SUB_PARAM_POS_VEC      = &quot;pos&quot;;
    
    //========================================
    // PluginConfigDebugProfileFixed
    //========================================
    void PluginConfigDebugProfileFixed()
    {
        m_ReadOnly = true;
