// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/helmetbase/motohelmet_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MotoHelmet_ColorBase extends HelmetBase
{
    override array<int> GetEffectWidgetTypes()
    {
        return {EffectWidgetsTypes.MOTO_OCCLUDER/*,EffectWidgetsTypes.MOTO_BREATH*/};
