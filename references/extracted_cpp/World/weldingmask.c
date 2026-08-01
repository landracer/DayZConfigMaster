// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/weldingmask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WeldingMask extends HelmetBase
{
    override array<int> GetEffectWidgetTypes()
    {
        return {EffectWidgetsTypes.HELMET_OCCLUDER/*,EffectWidgetsTypes.HELMET_BREATH*/};
