// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwincharstomach.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinCharStomach extends HudDebugWinBase
{
    TextListboxWidget               m_WgtValues;
    TextWidget                      m_WgtOverall;
    
    //============================================
    // Constructor
    //============================================
    void HudDebugWinCharStomach(Widget widget_root)
    {   
        m_WgtValues = TextListboxWidget.Cast( widget_root.FindAnyWidget(&quot;txl_StomachContents&quot;) );
        m_WgtOverall = TextWidget.Cast( widget_root.FindAnyWidget(&quot;InfoOverall&quot;) );
        //FitWindow();
