// Source: F:/Games/Dayz/scripts/3_game/undergroundarealoader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class JsonUndergroundTriggers
{
    ref array<ref JsonUndergroundAreaTriggerData> Triggers;
    
    static void SpawnParentedTriggers(EntityAI parent)
    {
        int networkIdLow, networkIdHigh;
        parent.GetNetworkID(networkIdLow, networkIdHigh);
        
        JsonUndergroundTriggers jsonData = UndergroundAreaLoader.GetData();
        if (jsonData && jsonData.Triggers)
        {
            foreach (int index, auto data : jsonData.Triggers)
            {
                if (data.CustomSpawn)
                {
                    if (data.ParentNetworkId.Count() == 2 && data.ParentNetworkId[0] == networkIdLow && data.ParentNetworkId[1] == networkIdHigh)
                        SpawnTriggerCarrier(parent, index, data);
                }
            }
        }
