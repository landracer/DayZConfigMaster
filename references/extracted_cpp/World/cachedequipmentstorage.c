// Source: F:/Games/Dayz/scripts/4_world/entities/cachedequipmentstorage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//#define CACHED_EQUIPMENT_STORAGE_LOGGING
 
class CachedEquipmentStorageInventoryEntry
{
    int m_SlotId = -1;
    
    string m_CallerMethod;
    EntityAI m_Entity;
    GameInventory m_GameInventory;
    
    ECachedEquipmentOperationMode m_OperationMode;
}
 
