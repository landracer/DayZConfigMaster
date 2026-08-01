// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsole.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsole extends UIScriptedMenu
{
    protected bool              m_HintEditMode;
    protected float             m_HoverTime;
    protected bool              m_HoverSuccessTriggered;
    protected Widget            m_CurrentHoverWidget;
    protected Widget            m_HintWidgetRoot;
    protected ImageWidget       m_HintWidgetBackground;
    protected Widget            m_EditTooltipRoot;
    protected RichTextWidget    m_HintWidget;
    protected ButtonWidget      m_HintOkButton;
    protected ButtonWidget      m_HintCancelButton;
    protected ButtonWidget      m_HintClearButton;
    protected EditBoxWidget     m_HintInputText;
    protected float             m_PrevMouseX;
    protected float             m_PrevMouseY;
    Widget                      m_ButtonsWindowWidget;
    ScriptConsoleTabBase        m_SelectedHandler;
    protected int               m_Id;
    
    static ref array<ref MapMarker>     m_MarkedEntities = new array<ref MapMarker>();
    int                                 m_SelectedTab;
    protected ref map<Widget, ref ScriptConsoleTabBase> m_TabHandlers = new map<Widget, ref ScriptConsoleTabBase>;
    protected ref map<int, ref ScriptConsoleTabBase> m_TabHandlersByID = new map<int, ref ScriptConsoleTabBase>;
 
    ButtonWidget                    m_CloseConsoleButton;
 
    
    protected static const string HINTS_PATH_DEFAULT        = &quot;scripts/data/internal/script_console_hints.json&quot;;
    protected static const string HINTS_PATH_OPTIONAL       = &quot;$mission:script_console_hints.json&quot;;
    
    static ref JsonHintsData m_JsonData;
    
    const string NO_HINT_TEXT = &quot;No hint&quot;;
    
    
    void ScriptConsole()
    {
        #ifndef SERVER
        if (g_Game && g_Game.GetMission() && g_Game.GetMission().GetHud())
        {
            g_Game.GetMission().GetHud().ShowHudPlayer(false);
            g_Game.GetMission().GetHud().ShowQuickbarPlayer(false);
        }
        #endif
        PluginItemDiagnostic plugin = PluginItemDiagnostic.Cast(GetPlugin(PluginItemDiagnostic));
        if (plugin)
            plugin.OnScriptMenuOpened(true);
