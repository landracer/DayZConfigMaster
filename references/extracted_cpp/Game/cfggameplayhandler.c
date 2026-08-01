// Source: F:/Games/Dayz/scripts/3_game/cfggameplayhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgGameplayHandler
{
    private static string m_Path = &quot;$mission:cfggameplay.json&quot;;
    private static ref array<ref ITEM_DataBase> m_Items = new array<ref ITEM_DataBase>();
    
    static ref CfgGameplayJson m_Data = new CfgGameplayJson();
    
    //---------------------------------------------------------------------------------------
    static void RegisterItem(ITEM_DataBase item)
    {
        m_Items.Insert(item);
