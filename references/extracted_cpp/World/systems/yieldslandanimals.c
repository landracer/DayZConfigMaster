// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/yielditems/yieldslandanimals.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LandAnimalYieldItemBase : YieldItemBase {}
 
//------------------------------------//
//generic animal with basic info only //
//------------------------------------//
class YieldItemGenericSmallAnimal : LandAnimalYieldItemBase
{
    void YieldItemGenericSmallAnimal(int baseWeight, string type, int enviroMask, int methodMask)
    {
        m_Type = type;
        m_EnviroMask = enviroMask;
        m_MethodMask = methodMask;
    }
}
 
//mammals
class YieldItemDeadRabbit : LandAnimalYieldItemBase
{
    override void Init()
    {
        super.Init();
        
        m_Type = &quot;DeadRabbit&quot;;
        m_EnviroMask = AnimalCatchingConstants.MASK_ENVIRO_FOREST;
        m_MethodMask = AnimalCatchingConstants.MASK_METHOD_LANDTRAP_SNARE;
