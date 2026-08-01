// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/pperotblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPERotBlur: PPEClassBase
{
    static const int PARAM_POWER = 0;
    static const int PARAM_MINANGLEPERSEC = 1;
    static const int PARAM_MAXANGLEPERSEC = 2;
    static const int PARAM_MINDEPTH = 3;
    static const int PARAM_MAXDEPTH = 4;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.RotBlur;
