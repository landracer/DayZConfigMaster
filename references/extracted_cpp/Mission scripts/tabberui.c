// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/tabberprefab/tabberui.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TabberUI extends ScriptedWidgetEventHandler
{
    protected bool                  m_FirstInit = true;
    protected Widget                m_Root;
    protected Widget                m_TabControlsRoot;
    
    protected int                   m_TabsCount;
    protected ref map<int, Widget>  m_TabControls;
    protected ref map<int, Widget>  m_Tabs;
    
    protected int                   m_SelectedIndex;
    protected float                 m_ResolutionMultiplier;
    protected bool                  m_CanSwitch;
    
    ref ScriptInvoker               m_OnTabSwitch = new ScriptInvoker();
    ref ScriptInvoker               m_OnAttemptTabSwitch = new ScriptInvoker();
    ref Timer                       m_InitTimer;
    
    protected void OnInputPresetChanged()
    {
        #ifdef PLATFORM_CONSOLE
        UpdateControlsElements();
        #endif
