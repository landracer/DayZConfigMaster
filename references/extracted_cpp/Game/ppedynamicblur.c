// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppedynamicblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEDynamicBlur: PPEClassBase
{
    static const int PARAM_BLURRINESS = 0;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.DynamicBlur;
