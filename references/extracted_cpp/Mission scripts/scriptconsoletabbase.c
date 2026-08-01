// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoletabbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleTabBase
{
    protected int m_Id;
    protected bool m_IsSelected;
    protected bool m_IsShiftDown;
    protected Widget m_ParentRoot;
    protected Widget m_Root;
    protected Widget m_Button;
    protected ScriptConsole m_ScriptConsole;
    protected ScriptConsoleTabBase m_Parent;//for subtabs
    protected PluginConfigDebugProfile m_ConfigDebugProfile;
    protected PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed;
    
    void ScriptConsoleTabBase(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_Button = button;
        m_ScriptConsole = console;
        m_Root = root;
        if (parent)
            m_ParentRoot = parent.GetRoot();
        else
            m_ParentRoot = console.layoutRoot;
        m_ConfigDebugProfile = PluginConfigDebugProfile.Cast(GetPlugin(PluginConfigDebugProfile));
        m_ConfigDebugProfileFixed   = PluginConfigDebugProfileFixed.Cast(GetPlugin(PluginConfigDebugProfileFixed));
