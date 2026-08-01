// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwinchardebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinCharDebug extends HudDebugWinBase
{
    private PluginDeveloper m_ModuleDeveloper;
    
    private TextWidget      m_PlayerPosTextWidget;
    private TextWidget      m_ClipboardTextWidget;
 
    //============================================
    // HudDebugWinCharDebug
    //============================================
    void HudDebugWinCharDebug(Widget widget_root)
    {   
        m_PlayerPosTextWidget = TextWidget.Cast( widget_root.FindAnyWidget(&quot;txt_PlayerPos&quot;) );
        m_ClipboardTextWidget = TextWidget.Cast( widget_root.FindAnyWidget(&quot;txt_Clipboard&quot;) );
