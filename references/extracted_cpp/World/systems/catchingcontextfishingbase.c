// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/catchingcontexts/catchingcontextfishingbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingContextFishingBase : CatchingContextBase
{
    bool m_IsSea;
    
    override protected void DeserializeData(Param par)
    {
        Param2<EntityAI,bool> p;
        if (Class.CastTo(p,par))
        {
            m_MainItem = p.param1;
            m_IsSea = p.param2;
            //m_Target = p.param2;
        }
