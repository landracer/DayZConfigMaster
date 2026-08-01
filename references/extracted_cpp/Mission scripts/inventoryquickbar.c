// Source: F:/Games/Dayz/scripts/5_mission/gui/inventory/inventoryquickbar.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InventoryQuickbar extends InventoryGridController
{
    
    protected ref TItemsMap m_Items;
    protected InventoryGrid m_Grid;
    protected int           m_DraggingIndex;
    
    void InventoryQuickbar(Widget quickbarGridWidget)
    {
        m_DraggingIndex = INDEX_NOT_FOUND;
        m_Items = new TItemsMap;
        UpdateItems( quickbarGridWidget );
