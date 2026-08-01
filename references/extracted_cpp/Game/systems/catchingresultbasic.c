// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchingresultbasic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingResultBasic
{
    protected EntityAI m_Owner;
    protected bool m_OverrideChanceActive = false; //do not perform chance updates
    protected bool m_OverrideQualityActive = false; //overrides quality
    protected float m_CatchChance = 1.0; //guaranteed catch by default
    protected float m_Quality = 1.0; //max quality default
    protected YieldItemBase m_YItem;
    
    void CatchingResultBasic(EntityAI owner)
    {
        m_Owner = owner;
