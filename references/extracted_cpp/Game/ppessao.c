// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppessao.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPESSAO: PPEClassBase
{
    static const int POSTPROCESS_OPTION_VALUE_LOW = 0;
    static const int POSTPROCESS_OPTION_VALUE_MEDIUM = 1;
    static const int POSTPROCESS_OPTION_VALUE_HIGH = 2;
    static const int POSTPROCESS_OPTION_VALUE_HIGHEST = 3;
    
    static const int PARAM_NUMPASSES = 0;
    static const int PARAM_INTENSITY = 1;
    static const int PARAM_RADIUSNEAR = 2;
    static const int PARAM_RADIUSFAR = 3;
    static const int PARAM_COSANGLE0 = 4;
    static const int PARAM_COSANGLE1 = 5;
    static const int PARAM_MAXDISTANCE = 6;
    static const int PARAM_MAXBLURDISTANCE = 7;
    static const int PARAM_NEARINTENSITY = 8;
    static const int PARAM_FARINTENSITY = 9;
    static const int PARAM_FARINTENSITYDIST = 10;
    static const int PARAM_NUMBLURPASSES = 11;
    static const int PARAM_BLURSIZE = 12;
    static const int PARAM_COLORAMOUNT = 13;
    static const int PARAM_COLORSSAOAMOUNT = 14;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.SSAO;
