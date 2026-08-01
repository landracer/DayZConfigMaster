// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoleoutputtab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleOutputTab : ScriptConsoleTabBase
{
    protected TextListboxWidget m_ClientLogListbox;
    protected ButtonWidget      m_ClientLogClearButton;
    protected CheckBoxWidget    m_ClientLogScrollCheckbox;
    
    void ScriptConsoleOutputTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_ClientLogListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;TextListbox&quot;));
        m_ClientLogClearButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonClear&quot;));
        m_ClientLogScrollCheckbox = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;CheckBoxAutoScroll&quot;));
        ReloadOutput();
