// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppedistort.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEDistort: PPEClassBase
{   
    static const int PARAM_DISTORT = 0;             
    static const int PARAM_CENTERX = 1;             
    static const int PARAM_CENTERY = 2;             
    static const int PARAM_CHROM_ABB = 3;           
    static const int PARAM_CHROM_ABB_SHIFTR = 4;    
    static const int PARAM_CHROM_ABB_SHIFTG = 5;    
    static const int PARAM_CHROM_ABB_SHIFTB = 6;    
    static const int PARAM_CHROM_COLOR_MOD = 7;     
    static const int PARAM_DISTORT_WEIGHT = 8;      
    
    //layering info
    static const int L_0_HMP = 100;
    static const int L_1_HMP = 100;
    static const int L_2_HMP = 100;
    static const int L_3_HMP = 100;
    static const int L_4_HMP = 100;
    static const int L_5_HMP = 100;
    static const int L_6_HMP = 100;
    static const int L_7_HMP = 100;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Distort;
