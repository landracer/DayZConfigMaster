// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoleenfscripttab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleEnfScriptTab : ScriptConsoleTabBase
{
    protected static ScriptConsoleEnfScriptTab  m_Instance;
    protected int                               m_EnscriptHistoryRow;
    protected int                               m_EnscriptHistoryRowServer;
    protected ref TStringArray                  m_EnscriptConsoleHistory;
    protected ref TStringArray                  m_EnscriptConsoleHistoryServer;
    protected PluginLocalEnscriptHistory        m_ModuleLocalEnscriptHistory;
    protected PluginLocalEnscriptHistoryServer  m_ModuleLocalEnscriptHistoryServer;
    protected MultilineEditBoxWidget            m_EnfScriptEdit;
    protected ButtonWidget                      m_EnfScriptRun;
    protected ButtonWidget                      m_EnfScriptClear;
    protected TextListboxWidget                 m_ScriptOutputListbox;
    protected bool                              m_AllowScriptOutput;
    protected int                               m_RunColor;
    
    static ref TStringArray                     m_ScriptOutputHistory = new TStringArray();
    
    void ScriptConsoleEnfScriptTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_Instance                          = this;
        m_ModuleLocalEnscriptHistory        = PluginLocalEnscriptHistory.Cast(GetPlugin(PluginLocalEnscriptHistory));
        m_ModuleLocalEnscriptHistoryServer  = PluginLocalEnscriptHistoryServer.Cast(GetPlugin(PluginLocalEnscriptHistoryServer));
        m_EnscriptConsoleHistory            = m_ModuleLocalEnscriptHistory.GetAllLines();
        m_EnscriptConsoleHistoryServer      = m_ModuleLocalEnscriptHistoryServer.GetAllLines();
        m_EnfScriptEdit                     = MultilineEditBoxWidget.Cast(root.FindAnyWidget(&quot;MultilineEditBoxWidget0&quot;));
        m_EnfScriptRun                      = ButtonWidget.Cast(root.FindAnyWidget(&quot;RunButton&quot;));
        m_EnfScriptClear                    = ButtonWidget.Cast(root.FindAnyWidget(&quot;ClearButton&quot;));
        m_ScriptOutputListbox               = TextListboxWidget.Cast(root.FindAnyWidget(&quot;ScriptOutputListbox&quot;));
        m_RunColor                          = m_EnfScriptRun.GetColor();
