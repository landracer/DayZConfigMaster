// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectormultistate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelectorMultistate extends OptionSelector
{
    protected bool  m_CanSwitch;
    
    void OptionSelectorMultistate(Widget parent, int current_index, ScriptedWidgetEventHandler parent_c, bool disabled, notnull array<string> options)
    {
        m_CanSwitch = true;
        
        m_SelectorType              = 2;
        m_Options                   = options;
        if (options.Count() == 0)
        {
            Error(&quot;Invalid OptionSelectorMultistate options&quot;);
        }
        
        if (current_index < 0 || current_index >= m_Options.Count())
        {
            m_SelectedOptionIndex = 0;
        }
        else
        {
            m_SelectedOptionIndex = current_index;
        }
        
        m_SelectedOption.SetText(m_Options.Get(m_SelectedOptionIndex));
