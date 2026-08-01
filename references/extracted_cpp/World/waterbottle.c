// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/bottle_base/waterbottle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WaterBottle extends Bottle_Base
{
    const float DAMAGE_OVERHEAT_PER_S = 0.1;
    const float DAMAGE_ENVIRO_LIQUID_COEF_MIN = 1;
    const float DAMAGE_ENVIRO_LIQUID_COEF_MAX = 2;
    const float DAMAGE_ENVIRO_TEMPDIFF_MIN = 80; //min damage at this demperature diff
    const float DAMAGE_ENVIRO_TEMPDIFF_MAX = 10; //maximum damage at this demperature diff
    
    override string GetPouringSoundset()
    {
        return &quot;emptyVessle_WaterBottle_SoundSet&quot;;
