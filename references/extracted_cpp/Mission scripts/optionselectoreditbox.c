// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectoreditbox.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelectorEditbox extends OptionSelectorBase
{
    protected EditBoxWidget         m_EditBox;
    
    void OptionSelectorEditbox(Widget parent, string value, ScriptedWidgetEventHandler parent_menu, bool disabled)
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/option_editbox.layout&quot;, parent);
        #ifdef PLATFORM_CONSOLE
            m_Parent                = parent.GetParent().GetParent();
        #else
        #ifdef PLATFORM_WINDOWS
            m_Parent                = parent.GetParent();
        #endif
        #endif
        
        m_SelectorType              = 1;
        m_ParentClass               = parent_menu;
        m_EditBox                   = EditBoxWidget.Cast(m_Root.FindAnyWidget(&quot;option_value&quot;));
        
        SetValue(value);
        Enable();
        
        m_Parent.SetHandler(this);
