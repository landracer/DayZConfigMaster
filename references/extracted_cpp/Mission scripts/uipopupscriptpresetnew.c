// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptpresetnew.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptPresetNew extends UIPopupScript
{
    private ButtonWidget m_BtnOk;
    private ButtonWidget m_BtnCancel;
    
    //================================================
    // UIPopupScriptSceneNew
    //================================================  
    void UIPopupScriptPresetNew( Widget wgt )
    {
        m_BtnOk     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pn_ok&quot;) );
        m_BtnCancel = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pn_cancel&quot;) );
