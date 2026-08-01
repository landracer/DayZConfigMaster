// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoleselector.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleSelector: OptionSelectorSliderSetup
{
    protected ScriptConsoleTabBase m_ParentTab;
    
    void ScriptConsoleSelector(Widget parent, float value, ScriptConsoleTabBase parent_tab, bool disabled, float min, float max)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(GetLayoutFile(), parent);      
        m_Parent = parent;
        m_SelectorType = 1;
        m_ParentTab = parent_tab;
        m_Slider = SliderWidget.Cast(m_Root.FindAnyWidget(&quot;option_value&quot;));
        m_Slider.SetCurrent(value);
 
        m_MinValue = min;
        m_MaxValue = max;
        
        SetValue(value);
        Enable();
        
        m_Parent.SetHandler(this);
