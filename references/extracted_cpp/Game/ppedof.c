// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/exceptions/ppedof.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//---------------------------------------------------------
//Native exceptions - legacy methods for direct access to specific postprocesses. Each one is evaluated and handled separately, this just connects them to the system.

class PPEDOF: PPEClassBase
{
    //g_Game.OverrideDOF(bool enable, float focusDistance, float focusLength, float focusLengthNear, float blur, float focusDepthOffset);
    static const int PARAM_ENABLE = 0;
    static const int PARAM_FOCUS_DIST = 1;
    static const int PARAM_FOCUS_LEN = 2;
    static const int PARAM_FOCUS_LEN_NEAR = 3;
    static const int PARAM_BLUR = 4;
    static const int PARAM_FOCUS_DEPTH_OFFSET = 5;
    
    static const int L_0_ADS = 100;
    static const int L_1_ADS = 100;
    static const int L_2_ADS = 100;
    static const int L_3_ADS = 100;
    static const int L_4_ADS = 100;
    static const int L_5_ADS = 100;
    
    override int GetPostProcessEffectID()
    {
        return PPEExceptions.DOF;
