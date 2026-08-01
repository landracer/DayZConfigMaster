// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppesmaa.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPESMAA: PPEClassBase
{
    static const int PARAM_PRESET = 0;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.SMAA;
