// Source: F:/Games/Dayz/scripts/4_world/systems/universaltemperaturesource/universaltemperaturesourcelambdabaseimpl.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalTemperatureSourceLambdaBaseImpl : UniversalTemperatureSourceLambdaBase
{
    override void DryItemsInVicinity(UniversalTemperatureSourceSettings pSettings, vector position, out notnull array<EntityAI> nearestObjects)
    {
        float distanceToTemperatureSource;
        
        foreach (Object nearestObject : nearestObjects)
        {
            ItemBase nearestItem = ItemBase.Cast(nearestObject);
            ItemBase temperatureSource = ItemBase.Cast(pSettings.m_Parent);

            if (nearestItem && nearestItem.HasWetness() && nearestItem != pSettings.m_Parent && !nearestItem.IsInherited(Man) && !nearestItem.IsUniversalTemperatureSource())
            {
                distanceToTemperatureSource = vector.Distance(nearestItem.GetPosition(), position); 
                distanceToTemperatureSource = Math.Max(distanceToTemperatureSource, 0.001); 
                distanceToTemperatureSource = 1 / distanceToTemperatureSource; 
                distanceToTemperatureSource = Math.Clamp(distanceToTemperatureSource, 0.0, 1.0); 
                
                float dryValue = 0;
                
                if (nearestItem.GetWet() >= GameConstants.STATE_DAMP)
                {
                    dryValue = (-1 * m_ExecuteInterval * nearestItem.GetDryingIncrement(&quot;groundHeatSource&quot;)) * pSettings.m_ItemDryModifier * distanceToTemperatureSource;
                    Math.Clamp(dryValue, nearestItem.GetWetMin(), nearestItem.GetWetMax());
                    nearestItem.AddWet(dryValue);
                }
                
                array<EntityAI> cargoEntities = new array<EntityAI>();
                nearestItem.GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, cargoEntities);
                foreach (EntityAI cargoEntity : cargoEntities)
                {
                    ItemBase cargoItem = ItemBase.Cast(cargoEntity);
                    if (cargoItem)
                    {
                        dryValue = 0;
                        if (cargoItem.GetWet() >= GameConstants.STATE_DAMP)
                        {
                            dryValue = (-1 * m_ExecuteInterval * cargoItem.GetDryingIncrement(&quot;groundHeatSource&quot;)) *  pSettings.m_ItemDryModifier * distanceToTemperatureSource;
                            Math.Clamp(dryValue, cargoItem.GetWetMin(), cargoItem.GetWetMax());
                            cargoItem.AddWet(dryValue);
                        }
                    }
                }
            }
        }
