// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/pperain.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPERain: PPEClassBase
{
    static const int PARAM_DROPDISTANCE = 0;
    static const int PARAM_DROPSIZEX = 1;
    static const int PARAM_DROPSIZEY = 2;
    static const int PARAM_RAINFOGGINESS = 3;
    static const int PARAM_RAINDENSITY = 4;
    static const int PARAM_FOGCOLORMULT = 5;
    static const int PARAM_BCKGRNDCOLORMULT = 6;
    static const int PARAM_REFRACTIONSCALE = 7;
    static const int PARAM_SUNVISIBLE = 8;
    static const int PARAM_GODRAYINTNEAR = 9;
    static const int PARAM_GODRAYINTFAR = 10;
    static const int PARAM_RIPPLESBLEND = 11;
    static const int PARAM_RIPPLESDIST = 12;
    static const int PARAM_SPEED = 13;
    static const int PARAM_DISTANTSPEED = 14;
    static const int PARAM_WINDAFFECTSPEED = 15;
    static const int PARAM_MINDIVERGENCE = 16;
    static const int PARAM_MAXDIVERGENCE = 17;
    static const int PARAM_WINDSPEEDFORMAXDIV = 18;
    /*
    static const int PARAM_DROPREFRACTIONTEX = 19;
    static const int PARAM_DISTANTTEX = 20;
    static const int PARAM_ROTNOISEMAP = 21;
    static const int PARAM_RIPPLEMAP = 22;
    static const int PARAM_MOVMAP = 23;
    static const int PARAM_MOVNOISEMAP = 24;
    static const int PARAM_SUNMASKMAT = 25;
    */
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.Rain;
