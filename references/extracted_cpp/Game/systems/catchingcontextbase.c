// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchingcontextbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BaitData
{
    float m_BaseProbability;
    EntityAI m_Owner;
    
    void BaitData(float value, EntityAI owner)
    {
        m_BaseProbability = value;
        m_Owner = owner;
