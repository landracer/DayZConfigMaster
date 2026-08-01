// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppeunderwater.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEUnderWater: PPEClassBase
{
    static const int PARAM_RSINTENSITY = 0;
    static const int PARAM_RSPLANEDIST = 1;
    static const int PARAM_RSDEPTHDECREASE = 2;
    static const int PARAM_RSUVSCALE = 3;
    static const int PARAM_WATERPLANELUM = 4;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.UnderWater;
