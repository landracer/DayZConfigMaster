// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppefxaa.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEFXAA: PPEClassBase
{
    static const int PARAM_PRESET = 0;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.FXAA;
