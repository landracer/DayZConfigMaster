// Source: F:/Games/Dayz/scripts/4_world/classes/arrowmanager/arrowmanagerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ArrowManagerBase
{
    const int VERSION = 1;
    protected ref array<EntityAI> m_Arrows;
    
    void ArrowManagerBase()
    {
        m_Arrows = new array<EntityAI>();
