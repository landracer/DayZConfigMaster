// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectorslider.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelectorSlider extends OptionSelectorSliderSetup
{
    protected bool m_Changed;
    protected EditBoxWidget m_ValueText;
    protected bool m_ShowEditbox;
    protected float m_LastValue;
 
    void OptionSelectorSlider(Widget parent, float value, ScriptedWidgetEventHandler parent_menu, bool disabled, float min, float max, bool showEditbox = false)
    {
        if (!showEditbox)
        {
            m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/option_slider.layout&quot;, parent);
        }
        else
        {
            m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/option_slider_editbox.layout&quot;, parent);
            m_ValueText = EditBoxWidget.Cast(m_Root.FindAnyWidget(&quot;option_value_text&quot;));
            m_ValueText.Enable(false);
            #ifdef PLATFORM_CONSOLE
            m_ValueText.SetFlags(WidgetFlags.IGNOREPOINTER);
            #endif
        }
 
        #ifdef PLATFORM_CONSOLE
        m_Parent = parent.GetParent().GetParent();
        #else
        #ifdef PLATFORM_WINDOWS
        m_Parent = parent.GetParent();
        #endif
        #endif
        
        m_MinValue = min;
        m_MaxValue = max;
        
        m_SelectorType = 1;
        m_ParentClass = parent_menu;
        m_Slider = SliderWidget.Cast(m_Root.FindAnyWidget(&quot;option_value&quot;));
        SetValue(value);
        m_LastValue = GetValue();
        
        m_ShowEditbox = showEditbox;
        
        Enable();
        
        if (showEditbox)
        {
            SetValueText();
        }
 
        m_Parent.SetHandler(this);
