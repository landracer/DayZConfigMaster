// ======================================================================
// File: carchingresulttrapbase_8c_source.html
// Category: other
// ======================================================================

class CatchingResultTrapBase : CatchingResultBase
{
override EntityAI SpawnAndSetup(out int yItemIdx, vector v = vector.Zero)
{
ItemBase catch = null;
if (Class.CastTo(catch,super.SpawnAndSetup(yItemIdx,v)))
{
catch.SetQuantityNormalized(m_Quality);
}
return catch;
}
}