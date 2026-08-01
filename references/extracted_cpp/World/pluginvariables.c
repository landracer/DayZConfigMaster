// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginvariables.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginVariables extends PluginBase
{
    void PluginVariables()
    {
        m_Id = 0;
        m_Variables = new map<int, string>;
        //              (&quot;variable name&quot;)
        RegisterVariable(&quot;varNote&quot;);
        RegisterVariable(&quot;varColor&quot;);
        
