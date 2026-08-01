// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/powergeneratorstatic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PowerGeneratorStatic : PowerGeneratorBase
{
    const int SWITCH_COUNT = 6;
    const int LOW_ENERGY_PERCENT = 5;
    
    protected static ref set<PowerGeneratorStatic> m_PowerGenerators = new set<PowerGeneratorStatic>();
    protected Land_WarheadStorage_PowerStation m_Parent;
    
    protected int m_ParentID1;
    protected int m_ParentID2;
    protected int m_ParentID3;
    protected int m_ParentID4;
    
    protected int m_LeverStatesBits;
    
    void PowerGeneratorStatic()
    {
        RegisterPersistentObject(this);
