// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptpresetrename.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptPresetRename extends UIPopupScript
{
    private ButtonWidget m_BtnOk;
    private ButtonWidget m_BtnCancel;
    private EditBoxWidget m_Editbox;
    
    //================================================
    // UIPopupScriptSceneNew
    //================================================  
    void UIPopupScriptPresetRename( Widget wgt )
    {
        m_BtnOk     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pr_ok&quot;) );
        m_BtnCancel = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pr_cancel&quot;) );
        m_Editbox = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;fld_ppp_pr_new_name&quot;) );
