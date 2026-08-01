// ======================================================================
// File: fox_8c_source.html
// Category: other
// ======================================================================

class DeadFox : Edible_Base
{
void DeadFox()
{
CacheSkinningBloodInfectionChance(eAgents.SALMONELLA);
}
override bool CanBeCookedOnStick()
{
return false;
}
override bool CanBeCooked()
{
return false;
}
override bool IsCorpse()
{
return true;
}
override bool CanDecay()
{
return true;
}
}