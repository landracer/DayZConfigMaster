// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptinitscript.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptInitScript extends UIPopupScript
{
    private MultilineEditBoxWidget  m_MedxInitScript;
    
    private ButtonWidget            m_BtnRun;
    private ButtonWidget            m_BtnSave;
    private ButtonWidget            m_BtnCancel;
    
    private int                     m_SceneObjectIndex;
    private SceneObject             m_SceneObject;
    
    private PluginSceneManager      m_ModuleSceneManager;
    
    //================================================
    // UIPopupScriptInitScript
    //================================================  
    void UIPopupScriptInitScript(Widget wgt)
    {
        m_ModuleSceneManager = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
        
        m_MedxInitScript    = MultilineEditBoxWidget.Cast( wgt.FindAnyWidget(&quot;pnl_ppp_is_init_script_value&quot;) );
        
        m_BtnRun            = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_is_run&quot;) );
        m_BtnSave           = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_is_save&quot;) );
        m_BtnCancel         = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_is_cancel&quot;) );
