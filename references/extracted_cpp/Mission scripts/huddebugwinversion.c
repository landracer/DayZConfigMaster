// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwinversion.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinVersion extends HudDebugWinBase
{
    private PluginDeveloper m_ModuleDeveloper;
    
    private TextWidget      m_VersionTextWidget;
 
    //============================================
    // HudDebugWinVersion
    //============================================
    void HudDebugWinVersion(Widget widget_root)
    {   
        m_VersionTextWidget = TextWidget.Cast( widget_root.FindAnyWidget(&quot;txt_Version&quot;) );
        
        string version;
        g_Game.GetVersion(version);
        m_VersionTextWidget.SetText(string.Format(&quot;exe: %1 | scripts: %2&quot;, version, GetPBOAPI().GetPBOVersion(&quot;dta\\scripts.pbo&quot;)));
