// Source: F:/Games/Dayz/scripts/3_game/particles/tests/pmtf.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PMTF : TestFramework
{
    private static int PM_CREATED = 0;
    private ref map<int, ref ParticleManager> m_Managers = new map<int, ref ParticleManager>();
    
    //---------------------------------------------------------------------------
    // Manager management
    //---------------------------------------------------------------------------
    int InsertManager(ParticleManager pm)
    {
        Assert(m_Managers.Insert(PM_CREATED, pm));
        ++PM_CREATED;
        
        return PM_CREATED - 1;
