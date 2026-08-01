// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptscenedelete.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptSceneDelete extends UIPopupScript
{
    private ButtonWidget    m_BtnYes;
    private ButtonWidget    m_BtnNo;
    private string          m_DeleteScene;
    
    //================================================
    // UIPopupScriptSceneDelete
    //================================================  
    void UIPopupScriptSceneDelete(Widget wgt)
    {
        m_BtnYes    = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_dlt_yes&quot;) );
        m_BtnNo     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_sn_dlt_no&quot;) );
