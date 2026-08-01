// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/thermometer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Thermometer extends ItemBase
{
    float GetTemperatureValue(PlayerBase player)
    {
        float value;
        if (player && player.IsSyncedModifierActive(eModifierSyncIDs.MODIFIER_SYNC_FEVER))
        {
            value = Math.RandomFloatInclusive(PlayerConstants.HIGH_TEMPERATURE_L, PlayerConstants.HIGH_TEMPERATURE_H);
        }
        else
        {
            value = Math.RandomFloatInclusive(PlayerConstants.NORMAL_TEMPERATURE_L, PlayerConstants.NORMAL_TEMPERATURE_H);
        }
        value = Math.Round(value * 10) / 10;
        return value;
