// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptpositionmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptPositionManager extends UIPopupScript
{
    private TextListboxWidget   m_LstPositionList;
    private EditBoxWidget m_TxtSelectedX;
    private EditBoxWidget m_TxtSelectedY;
    private EditBoxWidget m_TxtCurrentX;
    private EditBoxWidget m_TxtCurrentY;
    private PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed;
    private ButtonWidget m_TeleportButton;
    private ButtonWidget m_CancelButton;
            
    //================================================
    // UIPopupScriptPositionManager
    //================================================  
    void UIPopupScriptPositionManager( Widget wgt )
    {
        m_ConfigDebugProfileFixed   = PluginConfigDebugProfileFixed.Cast( GetPlugin( PluginConfigDebugProfileFixed ) );
        m_LstPositionList   = TextListboxWidget.Cast( wgt.FindAnyWidget(&quot;tls_ppp_pm_positions_list&quot;) );
        m_TxtSelectedX = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;pnl_ppp_pm_selected_x_value&quot;) );
        m_TxtSelectedY = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;pnl_ppp_pm_selected_y_value&quot;) );
        m_TxtCurrentX = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;pnl_ppp_pm_current_x_value&quot;) );
        m_TxtCurrentY = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;pnl_ppp_pm_current_y_value&quot;) );
        m_TeleportButton = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pm_teleport&quot;) );
        m_CancelButton = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pm_cancel&quot;) );
        /*
        TStringArray positions_array = new TStringArray;
        m_ConfigDebugProfileFixed.GetAllPositionsNames( positions_array );
        
        for ( int i = 0; i < positions_array.Count(); i++ )
        {
            m_LstPositionList.AddItem( positions_array.Get(i), new LocationParams ( positions_array.Get(i), false, vector.Zero), 0);
        }*/
