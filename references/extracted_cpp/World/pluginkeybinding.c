// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginkeybinding.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
const int MB_EVENT_PRESS            = 0;
const int MB_EVENT_CLICK            = 1;
const int MB_EVENT_RELEASE          = 2;
const int MB_EVENT_DOUBLECLICK      = 3;
const int MB_EVENT_DRAG             = 4;
 
class PluginKeyBinding extends PluginBase
{
    static PluginKeyBinding instance;
    
    // System Public Events
    void PluginKeyBinding()
    {
        if ( instance == null )
        {
            instance = this;
        }
