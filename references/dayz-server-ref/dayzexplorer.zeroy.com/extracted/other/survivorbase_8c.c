// ======================================================================
// File: survivorbase_8c_source.html
// Category: other
// ======================================================================

class SurvivorBaseType : PlayerBaseType
{
};
class SurvivorBase : PlayerBaseClient
{
void SurvivorBase()
{
CacheSkinningBloodInfectionChance(eAgents.SALMONELLA);
}
override int GetHideIconMask()
{
return EInventoryIconVisibility.HIDE_VICINITY;
}
}