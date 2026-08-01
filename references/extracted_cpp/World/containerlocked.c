// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/containerlocked.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContainerLockedBase : BuildingSuper
{
    protected int m_LockedMask = 0;
    protected int m_OpeningMask = 0;
    
    void ContainerLockedBase()
    {
        int count = GetDoorCount();
        for (int i = 0; i < count; i++)
        {
            LockDoor(i);
        }
