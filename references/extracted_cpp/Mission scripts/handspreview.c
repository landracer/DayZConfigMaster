// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/handspreview.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandsPreview: Container
{
    protected ref Icon  m_Icon;
    protected EntityAI  m_AttachmentsInitialized;
    
    protected float     m_IconSize;
 
    protected ItemBase  m_Item
 
    void HandsPreview( LayoutHolder parent )
    {
        g_Game.GetPlayer().GetOnItemAddedToHands().Insert( CreateNewIcon );
        g_Game.GetPlayer().GetOnItemRemovedFromHands().Insert( DeleteIcon );
