// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu/uipropertyattachment.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIPropertyAttachment
{
    private Widget          m_WgtRoot;
    private Widget          m_WgtThis;
    private XComboBoxWidget m_WgtComboBox;
    private TextWidget      m_WgtSlotName;
    private ref TStringArray m_ComboItems;
    private int             m_PrevIndex;
    private EntityAI        m_Obj;
    private int             m_SlotID;
    
    
    void UIPropertyAttachment(Widget root)
    {
        m_WgtRoot       = root;
        
        m_ComboItems = new TStringArray;
        
        m_WgtThis = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/scene_editor/day_z_scene_editor_attachment.layout&quot;, m_WgtRoot);
        m_WgtComboBox   = XComboBoxWidget.Cast( m_WgtThis.FindAnyWidget(&quot;combo_box&quot;) );
        m_WgtSlotName   = TextWidget.Cast( m_WgtThis.FindAnyWidget(&quot;txt_slot_name&quot;) );
