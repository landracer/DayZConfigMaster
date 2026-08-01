// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/playercontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerContainer: CollapsibleContainer
{
    protected ref AttachmentsGroupContainer     m_PlayerAttachmentsContainer;
    protected ref map<int, SlotsIcon>           m_InventorySlots;
    protected ref map<EntityAI, ref Container>  m_ShowedItems = new map<EntityAI, ref Container>;
    protected ref map<int, ref Container>       m_ShowedItemsIDs = new map<int, ref Container>;
    protected PlayerBase                        m_Player;
    
    protected const int                         HEADER_INDEX_OFFSET = 2;
    
    override void UpdateRadialIcon()
    {
        if ( m_SlotIcon )
        {
            bool show_radial_icon;
            show_radial_icon = IsHidden();
            Widget rip = m_SlotIcon.GetRadialIconPanel();
            rip.Show( !m_Player.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) && m_Player.CanDisplayCargo());
            SetOpenForSlotIcon(show_radial_icon);
        }
