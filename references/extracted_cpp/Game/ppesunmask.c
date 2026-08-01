// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppesunmask.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//A dummy class in script; &#39;SunMaskEffect&#39; is used as a part of Rain and GodRays
class PPESunMask: PPEClassBase
{
    /*
    static const int PARAM_INTENSITY = 0;
    static const int PARAM_SUNSIZE = 1;
    static const int PARAM_VSTREAKINT = 2;
    static const int PARAM_DSTREAKINT = 3;
    static const int PARAM_SUNMASK = 4;
    */
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.SunMask;
