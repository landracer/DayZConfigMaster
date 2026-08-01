// Source: F:/Games/Dayz/scripts/3_game/inventoryitemtype.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
//#define ITEM_SOUND_EVENTS_DEBUG
class InventoryItemSoundBase
{
    protected string m_Name;
    protected string m_CfgPath;
    protected ref map<int, string> m_AttachSoundSets;
    protected ref map<int, string> m_DetachSoundSets;
    
    void InventoryItemSoundBase(string name, string cfgPath)
    {
        m_Name = name;
        m_CfgPath = cfgPath;
                
        LoadSoundEvents();
