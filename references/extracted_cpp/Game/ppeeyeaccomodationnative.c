// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/exceptions/ppeeyeaccomodationnative.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//---------------------------------------------------------
//Native exceptions - legacy methods for direct access to specific postprocesses. Each one is evaluated and handled separately, this just connects them to the system.

class PPEEyeAccomodationNative: PPEClassBase
{
    //g_Game.GetWorld().SetEyeAccom
    static const int PARAM_INTENSITY = 0;
    
    static const int L_0_UNDERGROUND = 100;
    static const int L_0_NVG_GENERIC = 300;
    /*static const int L_0_NVG_OPTIC = 400;
    static const int L_0_NVG_GOGGLES = 401;
    static const int L_0_NVG_OFF = 402;*/
    static const int L_0_BURLAP = 500;
    
    override int GetPostProcessEffectID()
    {
        return PPEExceptions.EYEACCOM;
