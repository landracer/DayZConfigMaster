// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/exceptions/ppeexposurenative.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//---------------------------------------------------------
//Native exceptions - legacy methods for direct access to specific postprocesses. Each one is evaluated and handled separately, this just connects them to the system.

class PPEExposureNative: PPEClassBase
{
    //g_Game.SetEVValue
    static const int PARAM_INTENSITY = 0;
    
    static const int L_0_NVG_OPTIC = 100;
    static const int L_0_NVG_GOGGLES = 200;
    static const int L_0_NVG_OFF = 300;
    static const int L_0_FLASHBANG = 400;
    static const int L_0_BURLAP = 500;
    static const int L_0_DEATH = 1000;
    
    override int GetPostProcessEffectID()
    {
        return PPEExceptions.EXPOSURE;
