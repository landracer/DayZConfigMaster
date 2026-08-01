// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwincharlevels.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinCharLevels extends HudDebugWinBase
{
    TextListboxWidget               m_WgtValues;
    
    //============================================
    // Constructor
    //============================================
    void HudDebugWinCharLevels(Widget widget_root)
    {   
        m_WgtValues = TextListboxWidget.Cast( widget_root.FindAnyWidget(&quot;txl_CharLevels_Values&quot;) );
        
        FitWindow();
