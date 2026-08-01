// Source: F:/Games/Dayz/scripts/3_game/systems/arrowmanager/arrowmanagerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ArrowManagerBase
{
    protected const int VERSION = 2;
    protected ref array<EntityAI> m_Arrows;
    protected EntityAI m_Owner;
    
    void ArrowManagerBase(EntityAI owner)
    {
        m_Owner = owner;
        m_Arrows = new array<EntityAI>();
