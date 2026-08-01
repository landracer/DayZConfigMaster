// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchingconstants.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnimalCatchingConstants
{
    const float QUALITY_GENERIC_BASE = 1.0;
    const float QUALITY_FISH_BASE = 0.35;
    
    const float CHANCE_GENERIC_BASE = 1.0; //used for traps
    const float CHANCE_FISH_BASE = 1.0; //used for traps
    
    //masks
    //enviro mask
    static const int MASK_ENVIRO_POND = 1<<0;
    static const int MASK_ENVIRO_SEA = 1<<1;
    static const int MASK_ENVIRO_FOREST = 1<<2;
    static const int MASK_ENVIRO_FIELD = 1<<3;
    static const int MASK_ENVIRO_WATER_ALL = MASK_ENVIRO_POND|MASK_ENVIRO_SEA;
    static const int MASK_ENVIRO_LAND_ALL = MASK_ENVIRO_FOREST|MASK_ENVIRO_FIELD;
    
    //catch method mask
    static const int MASK_METHOD_ROD = 1<<0;
    static const int MASK_METHOD_FISHTRAP_LARGE = 1<<1;
    static const int MASK_METHOD_FISHTRAP_SMALL = 1<<2;
    static const int MASK_METHOD_LANDTRAP_SNARE = 1<<3;
    
    //Trapping constants, TODO: separate class?
    const float POISSON_CYCLE_MEAN_DEFAULT = 10.0;
