// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/plugindoorruler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginDoorRuler extends PluginBase
{
    PluginDeveloper m_ModuleDeveloper;
    ItemBase m_Ruler;
    ref Timer m_Timer;
    bool m_Initialized;
    
    void CheckInit()
    {
        if( FreeDebugCamera.GetInstance().IsActive() ) 
            Init();
