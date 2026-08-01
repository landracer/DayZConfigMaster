// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/pperadialblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPERadialBlur: PPEClassBase
{
    static const int PARAM_POWERX = 0;
    static const int PARAM_POWERY = 1;
    static const int PARAM_OFFSETX = 2;
    static const int PARAM_OFFSETY = 3;
    static const int PARAM_PIXELSCALE = 4;
    
    
    static const int L_0_PAIN_BLUR = 100;
    
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.RadialBlur;
