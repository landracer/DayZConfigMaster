// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/catchingresultstructures/carchingresulttrapbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

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
