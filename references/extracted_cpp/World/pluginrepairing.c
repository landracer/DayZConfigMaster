// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginrepairing.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginRepairing extends PluginBase
{
    bool Repair(PlayerBase player, ItemBase repair_kit, Object item, float specialty_weight, string damage_zone = &quot;&quot;, bool use_kit_qty = true)
    {
        switch (item.GetHealthLevel(damage_zone))
        {
            case GameConstants.STATE_PRISTINE:
                break;
            case GameConstants.STATE_RUINED:
                #ifdef DEVELOPER
                Debug.Log(&quot;repairing from GameConstants.STATE_RUINED&quot;);
                #endif
                CalculateHealth(player, repair_kit, item, specialty_weight, damage_zone, use_kit_qty);
                break;
            case GameConstants.STATE_WORN:
                if (CanRepairToPristine(player) || CanBeRepairedToPristine(item))
                {
                    CalculateHealth(player, repair_kit, item, specialty_weight,/* GameConstants.DAMAGE_PRISTINE_VALUE,*/ damage_zone, use_kit_qty);
                }
                break;
            default:
                CalculateHealth(player, repair_kit, item, specialty_weight, damage_zone, use_kit_qty);
                break;
        }
 
        return true;
