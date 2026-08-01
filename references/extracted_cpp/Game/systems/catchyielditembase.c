// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchyielditembase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class YieldItemBase
{
    protected string m_Type;
    protected int m_RegistrationIdx = -1; //mostly for sync purposes
    protected int m_BaseWeight; //occurence ratio in the enviroment
    protected float m_QualityBase = AnimalCatchingConstants.QUALITY_GENERIC_BASE;
    protected int m_EnviroMask; //which enviroment it is relevant for
    protected int m_MethodMask; //which catching method are we using
    //protected ref  m_BaitCompatibilityMask; //which baits are relevant for me
    
    void YieldItemBase(int baseWeight)
    {
        m_BaseWeight = baseWeight;
        
        Init();
