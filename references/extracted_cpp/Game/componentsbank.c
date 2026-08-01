// Source: F:/Games/Dayz/scripts/3_game/tools/componentsbank.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ComponentsBank
{
    private EntityAI    m_EntityParent;
    private ref Component   m_Components[COMP_TYPE_COUNT];
        
    void ComponentsBank(EntityAI entity_parent)
    {
        m_EntityParent = entity_parent;
