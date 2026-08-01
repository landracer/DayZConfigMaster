// Source: F:/Games/Dayz/scripts/4_world/classes/cooking/fireconsumable.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FireConsumable
{
    ItemBase m_Item;
    float m_Energy;
    float m_RemainingEnergy;
    
    void FireConsumable ( ItemBase item, float energy )
    {
        m_Item = item;
        m_Energy = energy;
        m_RemainingEnergy = energy;
