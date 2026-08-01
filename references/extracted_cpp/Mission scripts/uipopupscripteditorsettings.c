// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscripteditorsettings.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptEditorSettings extends UIPopupScript
{
    private CheckBoxWidget      m_WgtTglSeleHighlight;
    private CheckBoxWidget      m_WgtTglSavePlayerPos;
    private EditBoxWidget       m_EdxRotationDelta;
    
    private ButtonWidget        m_BtnCancel;
    
    private PluginSceneManager  m_ModuleSceneManager;
    
    //================================================
    // UIPopupScriptEditorSettings
    //================================================  
    void UIPopupScriptEditorSettings(Widget wgt)
    {
        m_ModuleSceneManager = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
        
        m_WgtTglSeleHighlight   = CheckBoxWidget.Cast( wgt.FindAnyWidget(&quot;cbx_ppp_est_flag_selection&quot;) );
        m_WgtTglSavePlayerPos   = CheckBoxWidget.Cast( wgt.FindAnyWidget(&quot;cbx_ppp_est_flag_load_player_pos&quot;) );
        m_EdxRotationDelta      = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;ebx_ppp_est_rotation_delta_value&quot;) );
        
        m_BtnCancel             = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_est_cancel&quot;) );
