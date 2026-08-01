// Source: F:/Games/Dayz/scripts/1_core/physics/physicsblock.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] PhysicsBlock;
class PhysicsBlock
{
    static proto PhysicsBlock Create(notnull IEntity ent1, notnull IEntity ent2);
    proto external void Remove(notnull IEntity worldEntity);
