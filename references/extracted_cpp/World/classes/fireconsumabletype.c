// Source: F:/Games/Dayz/scripts/4_world/classes/cooking/fireconsumabletype.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FireConsumableType
{
    typename    m_ItemType;
    float       m_Energy;
    bool        m_IsKindling;
    string      m_AttSlot;
    
    void FireConsumableType ( typename item_type, float energy, bool is_kindling, string att_slot )
    {
        m_ItemType = item_type;
        m_Energy = energy;
        m_IsKindling = is_kindling;
        m_AttSlot = att_slot;
