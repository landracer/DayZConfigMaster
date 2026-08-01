// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselector.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelector extends OptionSelectorBase
{
    protected Widget                        m_PreviousOption;
    protected Widget                        m_NextOption;
    protected TextWidget                    m_SelectedOption;
    protected int                           m_SelectedOptionIndex;
    protected ref array<string>             m_Options;
    
    void OptionSelector(Widget parent, int current_index, ScriptedWidgetEventHandler parent_c, bool disabled)
    {
        m_Options               = { &quot;#server_browser_disabled&quot;, &quot;#server_browser_show&quot;, &quot;#server_browser_hide&quot; };
        m_ParentClass           = parent_c;
        m_SelectorType          = 2;
        if (current_index < 0 || current_index >= m_Options.Count())
        {
            m_SelectedOptionIndex = 0;
        }
        else
        {
            m_SelectedOptionIndex = current_index;
        }
        
        m_Root                      = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/option_selector.layout&quot;, parent);
        #ifdef PLATFORM_CONSOLE
            m_Parent                = parent.GetParent().GetParent();
        #else
        #ifdef PLATFORM_WINDOWS
            m_Parent                = parent.GetParent();
        #endif
        #endif
        
        m_SelectedOption            = TextWidget.Cast(m_Root.FindAnyWidget(&quot;option_label&quot;));
        m_PreviousOption            = m_Root.FindAnyWidget(&quot;prev_option&quot;);
        m_NextOption                = m_Root.FindAnyWidget(&quot;next_option&quot;);
        
        #ifdef PLATFORM_CONSOLE
            m_NextOption.Show(false);
            m_PreviousOption.Show(false);
        #endif
        
        m_SelectedOption.SetText(m_Options.Get(m_SelectedOptionIndex));
        
        m_Enabled = !disabled;
        if (m_Enabled)
        {
            Enable();
        }
        else
        {
            Disable();
        }
        
        m_Parent.SetHandler(this);
