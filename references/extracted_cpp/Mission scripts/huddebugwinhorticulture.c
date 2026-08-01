// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptshuddebug/huddebugwinhorticulture.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HudDebugWinHorticulture : HudDebugWinBase
{
    protected EditBoxWidget m_FullMaturityEditBox;
    protected EditBoxWidget m_SpoilEditBox;
    protected EditBoxWidget m_SpoilRemoveEditBox;
    protected EditBoxWidget m_DryRemoveEditBox;
    protected EditBoxWidget m_SetAllEditBox;
    protected EditBoxWidget m_SpeedEditBox;
    protected ButtonWidget  m_ButtonUpdate;
    protected ButtonWidget  m_ButtonUpdateTarget;
    protected ButtonWidget  m_ButtonReset;
    protected ButtonWidget  m_ButtonSetAll;
    protected ButtonWidget  m_ButtonSpeed;
    
    protected PluginDeveloperSync   m_DevSyncPlugin;
    
    void HudDebugWinHorticulture(Widget widget_root)
    {   
        m_WgtRoot = widget_root;
