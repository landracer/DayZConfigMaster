// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsolegeneraltab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleGeneralTab : ScriptConsoleTabBase
{
    static int      m_ObjectsScope = 2;
    static protected vector     m_LastEditPos = vector.Zero;
    static protected vector     m_LastEditDir = vector.Zero;
    
    protected static float  DEBUG_MAP_ZOOM = 1;
    protected static bool   SHOW_OTHERS = 0;
    
    protected const string      NUMERIC_LETTERS = &quot;0123456789.-&quot;;
    protected vector            m_MapPos;
    protected bool              m_PlayerPosRefreshBlocked;
 
    protected ref array<ref RemotePlayerStatDebug> m_PlayerDebugStats = new array<ref RemotePlayerStatDebug>;
 
    protected MissionGameplay       m_MissionGameplay;
    protected PluginDeveloper       m_Developer;
 
    protected SliderWidget          m_TimeSlider;
    protected ref Timer             m_LateInit = new Timer();
    
    protected bool                  m_UpdatePlayerPositions;
    protected bool                  m_InitialOpen = true;
    
    //-------------------------------- WIDGETS ---------------------------------------
    protected EditBoxWidget         m_TeleportHeading;
    protected EditBoxWidget         m_TeleportXYZ;
    protected EditBoxWidget         m_DateYear;
    protected EditBoxWidget         m_DateMonth;
    protected EditBoxWidget         m_DateDay;
    protected EditBoxWidget         m_DateHour;
    protected EditBoxWidget         m_DateMinute;
 
    protected ButtonWidget      m_LocationAddButton;
    protected ButtonWidget      m_LocationRemoveButton;
    protected ButtonWidget      m_TeleportButton;
    protected ButtonWidget      m_ButtonCopyPos;
    protected ButtonWidget      m_DiagDrawButton;
    protected ButtonWidget      m_DiagToggleButton;
 
    protected CheckBoxWidget        m_LogsEnabled;
    protected CheckBoxWidget        m_HudDCharStats;
    protected CheckBoxWidget        m_HudDCharLevels;
    protected CheckBoxWidget        m_HudDCharStomach;
    protected CheckBoxWidget        m_HudDCharModifiers;
    protected CheckBoxWidget        m_HudDCharAgents;
    protected CheckBoxWidget        m_HudDCharDebug;
    protected CheckBoxWidget        m_HudDFreeCamCross;
    protected CheckBoxWidget        m_HudDVersion;
    protected CheckBoxWidget        m_HudDHealth;
    protected CheckBoxWidget        m_HudDHorticulture;
    
    protected CheckBoxWidget        m_ShowOthers;
    
    protected CheckBoxWidget        m_HudDTemperature;
    protected MapWidget             m_DebugMapWidget;
    
    protected TextWidget            m_PlayerCurPos;
    protected TextWidget            m_PlayerCurDir;
    protected TextWidget            m_MouseCurPos;
    protected TextWidget            m_PlayerMouseDiff;
    protected TextWidget            m_CameraCurPos;
    protected TextWidget            m_CameraCurDir;
 
    protected TextListboxWidget     m_DiagToggleTextListbox;
    protected TextListboxWidget     m_PositionsListbox;
    protected TextListboxWidget     m_DiagDrawmodeTextListbox;
    protected TextListboxWidget     m_HelpTextListboxWidget;
    
    protected FreeDebugCamera m_FreeDebugCamera;
    
    //-----------------------------------------------------------------------------------
    
    void ScriptConsoleGeneralTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_MissionGameplay           = MissionGameplay.Cast(g_Game.GetMission());
        
        m_DiagToggleTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;DiagToggle&quot;));
        m_DiagToggleButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;DiagToggleButton&quot;));
 
        m_DiagDrawmodeTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;DiagDrawmode&quot;));
        m_DiagDrawButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;DiagDrawButton&quot;));
 
        m_DebugMapWidget = MapWidget.Cast(root.FindAnyWidget(&quot;MapWidget&quot;));
        
        m_PositionsListbox  = TextListboxWidget.Cast(root.FindAnyWidget(&quot;PositionsList&quot;));
        m_TeleportButton    = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonTeleport&quot;));
        m_ButtonCopyPos     = ButtonWidget.Cast(root.FindAnyWidget(&quot;Button_CopyPos&quot;));
 
        m_TeleportHeading   = EditBoxWidget.Cast(root.FindAnyWidget(&quot;TeleportHeading&quot;));
        m_TeleportXYZ       = EditBoxWidget.Cast(root.FindAnyWidget(&quot;TeleportXYZ&quot;));
        m_PlayerCurPos      = TextWidget.Cast(root.FindAnyWidget(&quot;PlayerPosLabel&quot;));
        m_PlayerCurDir      = TextWidget.Cast(root.FindAnyWidget(&quot;PlayerDirLabel&quot;));
        m_PlayerMouseDiff   = TextWidget.Cast(root.FindAnyWidget(&quot;PlayerMouseDiff&quot;));
        m_MouseCurPos       = TextWidget.Cast(root.FindAnyWidget(&quot;MousePosLabel&quot;));
        m_CameraCurPos      = TextWidget.Cast(root.FindAnyWidget(&quot;CameraPosLabel&quot;));
        m_CameraCurDir      = TextWidget.Cast(root.FindAnyWidget(&quot;CameraDirLabel&quot;));
        m_LogsEnabled       = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_LogsEnabled&quot;));
        m_HudDCharStats     = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterStats&quot;));
        m_HudDCharLevels    = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterLevels&quot;));
        m_HudDCharStomach   = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterStomach&quot;));
        m_HudDCharModifiers = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterModifiers&quot;));
        m_HudDCharAgents    = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterAgents&quot;));
        m_HudDCharDebug     = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_CharacterDebug&quot;));
        m_HudDFreeCamCross  = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_FreeCamCross&quot;));
        m_HudDTemperature   = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_Temp&quot;));
        m_HudDVersion       = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_Version&quot;));
        m_HudDHealth        = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_Health&quot;));
        m_HudDHorticulture  = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;cbx_Horticulture&quot;));
        
        m_LocationAddButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;AddButton&quot;));
        //m_LocationAddButton.SetHandler(ToolTipEventHandler.GetInstance());
        m_LocationRemoveButton  = ButtonWidget.Cast(root.FindAnyWidget(&quot;RemoveButton&quot;));
 
        m_TimeSlider        = SliderWidget.Cast(root.FindAnyWidget(&quot;TimeSlider&quot;));
        m_DateYear          = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DateYear&quot;));
        m_DateMonth         = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DateMonth&quot;));
        m_DateDay           = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DateDay&quot;));
        m_DateHour          = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DateHour&quot;));
        m_DateMinute        = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DateMinute&quot;));
        
        
        m_ShowOthers        = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;ShowOthersCheckbox&quot;));
 
        m_HelpTextListboxWidget = TextListboxWidget.Cast(root.FindAnyWidget(&quot;HelpTextListboxWidget&quot;));
        m_Developer = PluginDeveloper.Cast(GetPlugin(PluginDeveloper));
        m_FreeDebugCamera = FreeDebugCamera.GetInstance();
        
        Init();
        LateInit();
