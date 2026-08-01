// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppechromaber.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEChromAber: PPEClassBase
{
    static const int PARAM_POWERX = 0;
    static const int PARAM_POWERY = 1;
    
    static const int L_0_INTRO = 100;
    static const int L_1_INTRO = 100;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.ChromAber;
