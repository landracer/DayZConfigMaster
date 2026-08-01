// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppecolors.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//TODO - &#39;ColorsEffect&#39; type may be used differently in c++, no emat linked to it? Investigate.
class PPEColors: PPEClassBase
{
    static const int PARAM_BRIGHTNESS = 0;
    static const int PARAM_CONTRAST = 1;
    static const int PARAM_OFFSET = 2;
    static const int PARAM_OVERLAYFACTOR = 3;
    static const int PARAM_OVERLAYCOLOR = 4;
    static const int PARAM_SATURATION = 5;
    static const int PARAM_COLORIZATIONCOLOR = 6;
    static const int PARAM_DESATURATIONWEIGHTS = 7;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Colors;
