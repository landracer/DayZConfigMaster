// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppeghost.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEGhost: PPEClassBase
{   
    //static const int PARAM_NOISE_TEX = 0; // changing texture not supported. Default &#39;0xff000000&#39;
    static const int PARAM_NOISE_CHANNEL_WEIGHT = 1;    
    static const int PARAM_NOISE_OFFSET_X = 2;          
    static const int PARAM_NOISE_OFFSET_Y = 3;          
    static const int PARAM_NOISE_SCALE_X = 4;           
    static const int PARAM_NOISE_SCALE_Y = 5;           
    static const int PARAM_NOISE_REMAP_LO = 6;          
    static const int PARAM_NOISE_REMAP_HI = 7;          
    
    static const int PARAM_GHOST_OFFSET_X = 8;          
    static const int PARAM_GHOST_OFFSET_Y = 9;          
    static const int PARAM_GHOST_SCALE_X = 10;          
    static const int PARAM_GHOST_SCALE_Y = 11;          
    static const int PARAM_GHOST_COLOR_MOD = 12;        
    
    //layering info
    static const int L_1_HMP = 100;
    static const int L_2_HMP = 100;
    static const int L_3_HMP = 100;
    static const int L_4_HMP = 100;
    static const int L_5_HMP = 100;
    static const int L_6_HMP = 100;
    static const int L_7_HMP = 100;
    static const int L_8_HMP = 100;
    static const int L_9_HMP = 100;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Ghost;
