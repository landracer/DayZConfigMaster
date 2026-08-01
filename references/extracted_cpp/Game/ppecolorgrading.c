// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppecolorgrading.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//TODO
class PPEColorGrading: PPEClassBase
{
    //static const int PARAM_COLORTABLE = 0;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.ColorGrading;
