// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindayzplayerdebug_othercmds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! PluginDayzPlayerDebug_OtherCmds
// *************************************************************************************
 
class PluginDayzPlayerDebug_OtherCmds
{   
    // widgets
    Widget                              m_MainWnd;
 
    XComboBoxWidget                     m_DeathTypeCB;
    EditBoxWidget                       m_DeathDirectionEdit;
    ButtonWidget                        m_DeathStartButton;
    
    XComboBoxWidget                     m_HitTypeCB;
    ButtonWidget                        m_HitStartButton;
 
    XComboBoxWidget                     m_UnconTypeCB;
    ButtonWidget                        m_UnconStartButton;
    ButtonWidget                        m_UnconEndButton;
    
    // command handler properties
    bool                                m_CH_DeathStart = false;
    bool                                m_CH_HitStart = false;
    bool                                m_CH_UnconStart = false;
    bool                                m_CH_UnconEnd   = false;
 
    
    //---------------------------------------------------
    // gui stuff
 
    void PluginDayzPlayerDebug_OtherCmds(Widget pMainWnd)
    {
        m_MainWnd = pMainWnd;
        CreateModuleWidgets();
