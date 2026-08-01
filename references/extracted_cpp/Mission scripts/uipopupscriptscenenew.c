// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptscenenew.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptSceneNew extends UIPopupScript
{
    private ButtonWidget m_BtnOk;
    private ButtonWidget m_BtnCancel;
    
    //================================================
    // UIPopupScriptSceneNew
    //================================================  
    void UIPopupScriptSceneNew(Widget wgt)
    {
        m_BtnOk     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_ok&quot;) );
        m_BtnCancel = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_cancel&quot;) );
