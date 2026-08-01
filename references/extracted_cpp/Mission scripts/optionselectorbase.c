// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/optionselectorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionSelectorBase extends ScriptedWidgetEventHandler
{
    protected int                           m_SelectorType = 0;
    protected Widget                        m_Parent;
    protected Widget                        m_Root;
    
    protected bool                          m_Enabled;
    
    protected ScriptedWidgetEventHandler    m_ParentClass;
    
    ref ScriptInvoker                       m_OptionFocused = new ScriptInvoker;
    ref ScriptInvoker                       m_OptionUnfocused = new ScriptInvoker;
    ref ScriptInvoker                       m_AttemptOptionChange = new ScriptInvoker;
    ref ScriptInvoker                       m_OptionChanged = new ScriptInvoker;
    
    void ~OptionSelectorBase()
    {
        delete m_Root;
