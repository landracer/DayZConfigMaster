// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptnotify.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptNotify extends UIPopupScript
{
    private ButtonWidget    m_BtnOk;
    private TextWidget      m_TxtLabel;
    
    //================================================
    // UIPopupScriptNotify
    //================================================  
    void UIPopupScriptNotify(Widget wgt)
    {
        m_BtnOk     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_notify_ok&quot;) );
        m_TxtLabel  = TextWidget.Cast( wgt.FindAnyWidget(&quot;txt_ppp_notify_label&quot;) );
