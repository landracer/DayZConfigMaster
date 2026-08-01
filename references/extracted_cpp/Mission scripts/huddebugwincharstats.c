// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwincharstats.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinCharStats extends HudDebugWinBase
{
    TextListboxWidget                       m_WgtValues;
    Widget                                  m_WgtPanel;
    ref array<ref Widget>                   m_StatWidgets = new array<ref Widget>;
    ref map <ref SliderWidget, string>      m_SliderWidgets = new map<ref SliderWidget, string>;
    ref array<ref TextWidget>               m_StatValues = new array<ref TextWidget>;
    ref map<ref EditBoxWidget, string>      m_StatValuesInput = new map<ref EditBoxWidget, string>;
    bool                                    m_Populated;
    bool                                    m_ChangingSlider;
    
    //============================================
    // Constructor
    //============================================
    void HudDebugWinCharStats(Widget widget_root)
    {
        m_WgtRoot = widget_root;
        m_WgtPanel = Widget.Cast(m_WgtRoot.FindAnyWidget(&quot;Stats&quot;) );
        //FitWindow();
