// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/eyepatch_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EyePatch_Improvised extends Glasses_Base
{
    override array<int> GetEffectWidgetTypes()
    {
        return {EffectWidgetsTypes.EYEPATCH_OCCLUDER};
