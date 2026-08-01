// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppefilmgrain.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEFilmGrain: PPEClassBase
{
    static const int PARAM_INTENSITY = 0;
    static const int PARAM_SHARPNESS = 1;
    static const int PARAM_GRAINSIZE = 2;
    static const int PARAM_INTENSITYX0 = 3;
    static const int PARAM_INTENSITYX1 = 4;
    static const int PARAM_MONOCHROMATIC = 5;
    static const int PARAM_SIMPLE = 6;
    static const int PARAM_DISTORT = 7;
    static const int PARAM_FREQUENCY = 8;
    //static const int PARAM_NOISEMAP = 9;
    
    static const int L_1_NVG = 100;
    static const int L_1_TOXIC_TINT = 200;
    static const int L_2_NVG = 100;
    static const int L_2_TOXIC_TINT = 200;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.FilmGrain;
