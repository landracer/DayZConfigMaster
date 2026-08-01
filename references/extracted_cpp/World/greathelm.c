// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/greathelm.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GreatHelm extends HelmetBase
{
    override array<int> GetEffectWidgetTypes()
    {
        return {EffectWidgetsTypes.HELMET_OCCLUDER/*,EffectWidgetsTypes.HELMET_BREATH*/};
