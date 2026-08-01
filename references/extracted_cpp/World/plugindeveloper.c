// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef Param7<EntityAI, string, float, float, bool, string, FindInventoryLocationType> DevSpawnItemParams;//(target, item_name, health, quantity, special, presetName, locationType );
class PluginDeveloper extends PluginBase
{
    protected bool                                  m_IsWinHolding; 
    protected int                                   m_FeaturesMask;
    UIScriptedMenu                                  m_ScriptConsole;
    
    static PluginDeveloper GetInstance()
    {
        return PluginDeveloper.Cast( GetPlugin( PluginDeveloper ) );
