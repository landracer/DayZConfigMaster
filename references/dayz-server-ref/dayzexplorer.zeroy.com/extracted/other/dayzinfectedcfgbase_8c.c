// ======================================================================
// File: dayzinfectedcfgbase_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------------------------
// DayZInfectedTypeCreate - main dayz type config
//-----------------------------------------------------------------------------------------------
void DayZInfectedTypeCreate(DayZInfectedType pType)
{
pType.RegisterAttacks();
pType.RegisterHitComponentsForAI();
}