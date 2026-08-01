// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindayzplayerdebug_ctrl.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
 
// *************************************************************************************
// ! PluginDayzPlayerDebug_Ctrl
// *************************************************************************************
class PluginDayzPlayerDebug_Ctrl
{   
    Widget                                  m_MainWnd;
 
    XComboBoxWidget                         m_ForceStance;
    EditBoxWidget                           m_MovSpeedEB;
    EditBoxWidget                           m_MovDirEB;
    CheckBoxWidget                          m_MovOverrideCheckbox;
    // 
 
    int                                     m_ApplyStanceChange;
    bool                                    m_OverrideMovementChange = false;
 
 
    //---------------------------------------------------
  // gui stuff
 
    void PluginDayzPlayerDebug_Ctrl(Widget pMainWnd)
    {
        m_MainWnd           = pMainWnd;
        m_ApplyStanceChange = -2;
 
        CreateModuleWidgets();
