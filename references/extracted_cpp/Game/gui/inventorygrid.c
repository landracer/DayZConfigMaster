// Source: F:/Games/Dayz/scripts/3_game/gui/inventorygrid.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class InventoryGridController extends ScriptedWidgetEventHandler
{
    void OnItemEnter(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemLeave(InventoryGrid grid, Widget w) {}
    void OnItemDrag(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemDrop(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemClick(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemLeftClick(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col) {}
    void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col) {}
    // float GetItemQuantity(InventoryGrid grid, InventoryItem item) {}
    int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
    { 
        return 0;
