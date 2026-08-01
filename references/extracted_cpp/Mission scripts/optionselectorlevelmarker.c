// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectorlevelmarker.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class OptionSelectorLevelMarker extends OptionSelectorSliderSetup
{
    protected SliderWidget      m_Slider2;
    
    void OptionSelectorLevelMarker(Widget parent, float value, ScriptedWidgetEventHandler parentMenu, bool disabled, float min, float max)
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/option_level_marker.layout&quot;, parent);
        #ifdef PLATFORM_CONSOLE
            m_Parent                = parent.GetParent().GetParent();
        #else
        #ifdef PLATFORM_WINDOWS
            m_Parent                = parent.GetParent();
        #endif
        #endif
        
        m_SelectorType          = 1;
        m_ParentClass           = parentMenu;
        m_Slider                = SliderWidget.Cast(m_Root.FindAnyWidget(&quot;marker_value&quot;));
        m_Slider2               = SliderWidget.Cast(m_Root.FindAnyWidget(&quot;other_slider_value&quot;));
        
        m_MinValue                  = min;
        m_MaxValue                  = max;
        
        SetValue(value, false);
        SetSlider2Value(0);
        Enable();
 
        m_Parent.SetHandler(this);
