// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/yielditems/yieldsfish.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FishYieldItemBase : YieldItemBase
{
    //used for interpolation between min/max times (UAFishingConstants.CYCLE_LENGTH_MIN/UAFishingConstants.CYCLE_LENGTH_MAX)
    protected float m_HourlyCycleLengthCoefs[24]; //0 - 23h;
    
    override void Init()
    {
        super.Init();
        
        m_HourlyCycleLengthCoefs = {1,1,1,1,0.5,0.2,0,0,0,0.2,0.5,1,1/*12h*/,1,1,1,0.75,0.5,0.2,0,0,0,0.2,0.5};
        m_QualityBase = AnimalCatchingConstants.QUALITY_FISH_BASE; //base quality of the fish
