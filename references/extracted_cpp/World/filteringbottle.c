// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/bottle_base/filteringbottle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FilteringBottle: Bottle_Base
{
    const float DAMAGE_CONSUME_PER_ML = 0.01;
    const float DAMAGE_OVERHEAT_PER_S = 1;
    const float DAMAGE_ENVIRO_LIQUID_COEF_MIN = 1;
    const float DAMAGE_ENVIRO_LIQUID_COEF_MAX = 1.5;
    const float DAMAGE_ENVIRO_TEMPDIFF_MIN = 80; //min damage at this demperature diff
    const float DAMAGE_ENVIRO_TEMPDIFF_MAX = 10; //maximum damage at this demperature diff
    
    override string GetPouringSoundset()
    {
        return &quot;emptyVessle_WaterBottle_SoundSet&quot;;
