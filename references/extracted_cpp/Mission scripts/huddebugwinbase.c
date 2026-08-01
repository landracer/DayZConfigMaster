// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwinbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinBase
{
    Widget m_WgtRoot;
    protected bool m_Show;
    protected bool m_RPCSent;
    
    //============================================
    // Constructor
    //============================================
    void HudDebugWinBase( Widget widget_root )
    {
        m_WgtRoot = widget_root;
