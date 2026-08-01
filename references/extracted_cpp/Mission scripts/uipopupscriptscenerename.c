// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptscenerename.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptSceneRename extends UIPopupScript
{
    private ButtonWidget    m_BtnOk;
    private ButtonWidget    m_BtnCancel;
    private string          m_RenameName;
    
    //================================================
    // UIPopupScriptSceneRename
    //================================================  
    void UIPopupScriptSceneRename(Widget wgt)
    {
        m_BtnOk         = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_rnm_ok&quot;) );
        m_BtnCancel     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_rnm_cancel&quot;) );
