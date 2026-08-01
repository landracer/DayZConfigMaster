// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matclasses/ppedepthoffield.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//TODO - may be just a dummy, since CGame.OverrideDOF function handles script overrides?
class PPEDepthOfField: PPEClassBase
{
    static const int PARAM_DOFLQ = 0;
    static const int PARAM_FOCALDISTANCE = 1;
    static const int PARAM_HYPERFOCAL = 2;
    static const int PARAM_FOCALOFFSET = 3;
    static const int PARAM_BLURFACTOR = 4;
    static const int PARAM_SIMPLEDOF = 5;
    static const int PARAM_SIMPLEHFNEAR = 6;
    static const int PARAM_SIMPLEDOFSIZE = 7;
    static const int PARAM_SIMPLEDOFGAUSS = 8;
    
    override int GetPostProcessEffectID()
    {
        return PostProcessEffectType.DepthOfField;
