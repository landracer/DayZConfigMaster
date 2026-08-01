// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsolepresetslist.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsolePresetsList extends ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected Widget m_Parent;
    protected ScriptConsoleTabBase m_Tab;
    protected TextListboxWidget m_PresetsList;
    
    ref ScriptInvoker m_ItemSelected = new ScriptInvoker;
    
    void ScriptConsolePresetsList(Widget parent, ScriptConsoleTabBase tab)
    {
        m_Parent = parent;
        m_Tab = tab;
 
        m_Root = g_Game.GetWorkspace().CreateWidgets(GetLayoutFile(), parent);
        m_PresetsList = TextListboxWidget.Cast(m_Root);
        
        m_Root.SetHandler(this);
