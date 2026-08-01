// ======================================================================
// File: physicsworld_8c_source.html
// Category: other
// ======================================================================

sealed class PhysicsWorld
{
private void PhysicsWorld();
private void ~PhysicsWorld();
static proto void SetInteractionLayer(notnull IEntity worldEntity, int mask1, int mask2, bool enable);
static proto vector GetGravity(notnull IEntity worldEntity);
static proto void SetGravity(notnull IEntity worldEntity, vector g);
static proto float GetTimeSlice(notnull IEntity worldEntity);
static proto int GetUpdateRate(notnull IEntity worldEntity);
static proto void SetUpdateRate(notnull IEntity worldEntity, int updateRate);
static proto int GetNumDynamicBodies(notnull IEntity worldEntity);
static proto IEntity GetDynamicBody(notnull IEntity worldEntity, int index);
}