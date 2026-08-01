// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppeglow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*enum PPEGlow
{
    PARAM_VISIBLEPERCENT = 0,
}*/

class PPEGlow: PPEClassBase
{
    static const int PARAM_VISIBLEPERCENT = 0;
    static const int PARAM_TARGETBRIGHTNESS = 1;
    static const int PARAM_SPEEDDARKTOBRIGHT = 2;
    static const int PARAM_SPEEDBRIGHTTODARK = 3;
    static const int PARAM_TONEMAPPING = 4;
    static const int PARAM_HDR = 5;
    static const int PARAM_FILMICSHOULDERSTRENGTH = 6;
    static const int PARAM_FILMICLINEARSTRENGTH = 7;
    static const int PARAM_FILMICLINEARANGLE = 8;
    static const int PARAM_FILMICTOESTRENGTH = 9;
    static const int PARAM_FILMICTOENUMERATOR = 10;
    static const int PARAM_FILMICTOEDENUMERATOR = 11;
    static const int PARAM_FILMICEXPOSUREBIAS = 12;
    static const int PARAM_FILMICWHITEPOINT = 13;
    static const int PARAM_BLOOMTHRESHOLD = 14;
    static const int PARAM_BLOOMSTEEPNESS = 15;
    static const int PARAM_BLOOMINTENSITY = 16;
    static const int PARAM_BRIGHTNESS = 17;
    static const int PARAM_CONTRAST = 18;
    static const int PARAM_OFFSET = 19;
    static const int PARAM_OVERLAYFACTOR = 20;
    static const int PARAM_OVERLAYCOLOR = 21;
    static const int PARAM_SATURATION = 22;
    static const int PARAM_COLORIZATIONCOLOR = 23;
    static const int PARAM_DESATURATIONWEIGHTS = 24;
    static const int PARAM_VIGNETTE = 25;
    static const int PARAM_VIGNETTECOLOR = 26;
    static const int PARAM_LENSDISTORT = 27;
    static const int PARAM_MAXCHROMABBERATION = 28;
    static const int PARAM_LENSCENTERX = 29;
    static const int PARAM_LENSCENTERY = 30;
    
    //layer info
    static const int L_20_HIT = 100;
    static const int L_20_FLASHBANG = 300;
    static const int L_20_SHOCK = 500;
    
    static const int L_21_SHOCK = 100;
    static const int L_21_HIT = 300;
    static const int L_21_FLASHBANG = 500;
    
    static const int L_22_BLOODLOSS = 100;
    
    static const int L_23_GLASSES = 100;
    static const int L_23_TOXIC_TINT = 200;
    static const int L_23_HMP = 300;
    static const int L_23_NVG = 600;
    
    static const int L_25_MENU = 100;
    static const int L_25_TUNNEL = 300;
    static const int L_25_SHOCK = 500;
    static const int L_25_UNCON = 700;
    static const int L_25_BURLAP = 800;
    
    static const int L_26_MENU = 100;
    static const int L_26_TUNNEL = 300;
    static const int L_26_SHOCK = 500;
    static const int L_26_UNCON = 700;
    static const int L_26_BURLAP = 800;
    
    static const int L_27_ADS = 100;
    
    static const int L_28_ADS = 100;
    
    static const int L_29_ADS = 100;
    
    static const int L_30_ADS = 100;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Glow;
