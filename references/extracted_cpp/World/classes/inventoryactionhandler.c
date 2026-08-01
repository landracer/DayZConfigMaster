// Source: F:/Games/Dayz/scripts/4_world/classes/inventoryactionhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class InventoryActionHandler
{
    ActionBase m_action;
    ActionTarget m_target;
    ItemBase m_mainItem;
    bool    m_useItemInHands;
 
    PlayerBase m_player;
    
    bool m_isActive;
    vector m_actionStartPos;
    
    const float MIN_DISTANCE_TO_INTERRUPT = 1.0;    
    const int IAH_SINGLE_USE = 1;
    const int IAH_CONTINUOUS = 2;
 
 
    void InventoryActionHandler(PlayerBase player)
    {
        m_player = player;
        m_isActive = false;
        m_action = null;
        m_target = null;
        m_mainItem = null;
        m_useItemInHands = false;
 
