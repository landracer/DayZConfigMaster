// ======================================================================
// File: rabbit_8c_source.html
// Category: other
// ======================================================================

class DeadRabbit : Edible_Base
{
void DeadRabbit()
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