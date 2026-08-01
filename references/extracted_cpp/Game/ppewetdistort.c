// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppewetdistort.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEWetDistort: PPEClassBase
{
    static const int PARAM_BLURPOWER = 0;
    static const int PARAM_LOCALBLURPOWER = 1;
    static const int PARAM_EFFPOWERTOP = 2;
    static const int PARAM_EFFPOWERBOTTOM = 3;
    static const int PARAM_BLURDOWNSIZE = 4;
    static const int PARAM_BLURGAUSS = 5;
    static const int PARAM_WAVSPDX1 = 6;
    static const int PARAM_WAVSPDX2 = 7;
    static const int PARAM_WAVSPDY1 = 8;
    static const int PARAM_WAVSPDY2 = 9;
    static const int PARAM_WAVEAMPX1 = 10;
    static const int PARAM_WAVEAMPX2 = 11;
    static const int PARAM_WAVEAMPY1 = 12;
    static const int PARAM_WAVEAMPY2 = 13;
    static const int PARAM_PHASERANDX = 14;
    static const int PARAM_PHASERANDY = 15;
    static const int PARAM_PHASEPOSX = 16;
    static const int PARAM_PHASEPOSY = 17;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.WetDistort;
