// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipopupscript/uipopupscriptconfigs.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPopupScriptConfigs extends UIPopupScript
{
    private ButtonWidget m_BtnOk;
    private ButtonWidget m_BtnCancel;
    private ButtonWidget m_BtnCopyToClipboard;
    
    private TextListboxWidget m_ConfigHierarchyTextListbox;
    private TextListboxWidget m_ConfigVariablesTextListbox;
    
    private PluginConfigViewer m_ModuleConfigViewer;
    
    private EditBoxWidget m_ObjectConfigFilter;
    
    void UIPopupScriptConfigs( Widget wgt )
    {
        m_BtnOk     = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pc_ok&quot;) );
        m_ConfigHierarchyTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget(&quot;ConfigHierarchy&quot;) );
        m_ConfigVariablesTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget(&quot;ConfigVariables&quot;) );
        m_BtnCopyToClipboard = ButtonWidget.Cast( wgt.FindAnyWidget(&quot;btn_ppp_pc_copy&quot;) );
        m_ObjectConfigFilter = EditBoxWidget.Cast( wgt.FindAnyWidget(&quot;ObjectConfigFilter&quot;) );
