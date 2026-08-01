// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppemedian.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//unused/not modifiable from script?
class PPEMedian: PPEClassBase
{
    static const int PARAM_KERNEL = 0;
    static const int PARAM_METHOD = 1;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Median;
