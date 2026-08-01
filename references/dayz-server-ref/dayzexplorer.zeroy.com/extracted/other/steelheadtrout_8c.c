// ======================================================================
// File: steelheadtrout_8c_source.html
// Category: other
// ======================================================================

class SteelheadTrout extends Edible_Base
{
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