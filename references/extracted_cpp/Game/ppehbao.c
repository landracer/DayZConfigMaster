// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppehbao.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//TODO - may be just a dummy, since SSAO already uses the HBAO materials?
class PPEHBAO: PPEClassBase
{
    static const int PARAM_RADIUSMETERS = 0;
    static const int PARAM_INTENSITY = 1;
    static const int PARAM_BLURSHARPNESS = 2;
    static const int PARAM_NDOTVBIAS = 3;
    static const int PARAM_SMALLSCALEAO = 4;
    static const int PARAM_LARGESCALEAO = 5;
    static const int PARAM_NUMDIRECTIONS = 6;
    static const int PARAM_NUMSAMPLES = 7;
    static const int PARAM_DEINTERLEAVE = 8;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.HBAO;
