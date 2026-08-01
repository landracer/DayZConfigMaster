// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/catchingcontexts/catchingcontexttraps.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingContextTrapsBase : CatchingContextBase
{
    protected float m_CumulativeTrappingSuccess;
    protected int m_AttemptsCount;
    protected ItemBase m_Bait;
    
    override protected void DeserializeData(Param par)
    {
        Param2<EntityAI,int> p;
        if (Class.CastTo(p,par))
        {
            m_MainItem = p.param1;
            m_AttemptsCount = p.param2;
        }
