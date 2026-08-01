// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginBase
{
    void PluginBase();  
    void ~PluginBase();
    
    void OnInit();  
    void OnUpdate(float delta_time);    
    void OnDestroy();
    
    void Log(string msg, string label)
    {
        Debug.Log(msg, GetModuleName(), &quot;n/a&quot;, label, &quot;n/a&quot;);
