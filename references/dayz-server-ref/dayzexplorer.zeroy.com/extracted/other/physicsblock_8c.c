// ======================================================================
// File: physicsblock_8c_source.html
// Category: other
// ======================================================================

typedef int[] PhysicsBlock;
class PhysicsBlock
{
static proto PhysicsBlock Create(notnull IEntity ent1, notnull IEntity ent2);
proto external void Remove(notnull IEntity worldEntity);
}