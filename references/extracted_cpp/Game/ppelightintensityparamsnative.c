// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/exceptions/ppelightintensityparamsnative.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//---------------------------------------------------------
//Native exceptions - legacy methods for direct access to specific postprocesses. Each one is evaluated and handled separately, this just connects them to the system.

class PPELightIntensityParamsNative: PPEClassBase
{
    //g_Game.SetEVValue
    static const int PARAM_LIGHT_MULT = 0;
    static const int PARAM_NOISE_MULT = 1;
    
    static const int L_0_NVG = 100;
    static const int L_0_TOXIC_TINT = 200;
    static const int L_1_NVG = 100;
    static const int L_1_TOXIC_TINT = 200;
    
    override int GetPostProcessEffectID()
    {
        return PPEExceptions.NVLIGHTPARAMS;
