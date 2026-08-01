// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/attachmentcategoriescontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachmentCategoriesContainer: CollapsibleContainer
{
    protected ref map<string, int>                          m_CategorySlotIndex;
    protected int                                           m_SlotsCount;
    protected SlotsIcon                                     m_CargoSlotsIcon;
 
    void AttachmentCategoriesContainer( LayoutHolder parent, int sort = -1 )
    {
        m_CategorySlotIndex = new map<string, int>;
