// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwintemperature.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinTemperature extends HudDebugWinBase
{
    private PluginDeveloper         m_ModuleDeveloper;
    
    private TextWidget              m_EnviroTextWidget;
    protected PluginDeveloperSync   m_PluginDeveloperSync;
    //============================================
    // HudDebugWinTemperature
    //============================================
    void HudDebugWinTemperature(Widget widget_root)
    {   
        m_EnviroTextWidget = TextWidget.Cast( widget_root.FindAnyWidget(&quot;txt_Temp&quot;) );
        m_PluginDeveloperSync = PluginDeveloperSync.Cast( GetPlugin( PluginDeveloperSync ) );
        
