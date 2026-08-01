// Source: F:/Games/Dayz/scripts/5_mission/gui/respawndialogue.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RespawnDialogue extends UIScriptedMenu
{
    const int                   ID_RESPAWN_CUSTOM = 101;
    const int                   ID_RESPAWN_RANDOM = 102;
    
    //tooltips
    protected Widget            m_DetailsRoot;
    protected TextWidget        m_DetailsLabel;
    protected RichTextWidget    m_DetailsText;
    
    protected Widget            m_CustomRespawn;
    
    //helper
    protected Widget            m_CurrentlyHighlighted;
    
    void RespawnDialogue();
    void ~RespawnDialogue();
    
    override Widget Init()
    {
        layoutRoot                  = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_respawn_dialogue.layout&quot;);
        m_DetailsRoot               = layoutRoot.FindAnyWidget(&quot;menu_details_tooltip&quot;);
        m_DetailsLabel              = TextWidget.Cast(m_DetailsRoot.FindAnyWidget(&quot;menu_details_label&quot;));
        m_DetailsText               = RichTextWidget.Cast(m_DetailsRoot.FindAnyWidget(&quot;menu_details_tooltip_content&quot;));
        
        m_CustomRespawn             = layoutRoot.FindAnyWidget(&quot;respawn_button_custom&quot;);
        SetFocus(m_CustomRespawn);
 
        return layoutRoot;
