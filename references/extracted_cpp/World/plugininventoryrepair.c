// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugininventoryrepair.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! PluginInventoryRepair
// *************************************************************************************
class PluginInventoryRepair extends PluginBase
{
    protected static float PIR_REQUEST_COOLDOWN = 5;
    protected static float PIR_MAX_ITEMS_TO_SEND = 5;
    
    protected ref array<EntityAI> m_ItemsToRepair;
    protected ref array<ref InventoryLocation> m_CorrectILs;
    protected float m_TimeFromLastRequest = 0;
    
    void PluginInventoryRepair()
    {
        m_ItemsToRepair = new array<EntityAI>();
        m_CorrectILs = new array<ref InventoryLocation>();
