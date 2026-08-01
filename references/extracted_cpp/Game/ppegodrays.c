// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppegodrays.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEGodRays: PPEClassBase
{
    static const int PARAM_INTENSITY = 0;
    static const int PARAM_SUNVISIBLE = 1;
    static const int PARAM_OVERBURNINTENSITY = 2;
    static const int PARAM_OVERBURNSTART = 3;
    static const int PARAM_OVERBURNEND = 4;
    //static const int PARAM_SUNMASKMAT = 5; //Warning, not to be used until DECLARE_RESOURCE_NAME solution is implemented!
    
    static const int L_0_GLASSES = 100;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.GodRays;
