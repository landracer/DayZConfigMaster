// ======================================================================
// File: aiworld_8c_source.html
// Category: other
// ======================================================================

enum PGPolyFlags
{
NONE,
WALK, // Ability to walk (ground, grass, road)
DISABLED, // Disabled polygon
DOOR, // Ability to move through doors
INSIDE, // Ability to move inside buildings
SWIM, // Ability to swim (water)
SWIM_SEA, // Ability to swim (sea water)
LADDER, // Ability to climb on ladders
JUMP_OVER, // Ability to do jumps overs
JUMP_DOWN, // Ability to jump down
CLIMB, // Ability to climb up
CRAWL, // Ability to crawl
CROUCH, // Ability to crouch
UNREACHABLE,
ALL,
JUMP, // JUMP_OVER | JUMP_DOWN
SPECIAL // JUMP | CLIMB | CRAWL | CROUCH
}
enum PGAreaType
{
NONE,
TERRAIN,
WATER,
WATER_DEEP,
WATER_SEA,
WATER_SEA_DEEP,
OBJECTS_NOFFCON,
OBJECTS,
BUILDING,
ROADWAY,
TREE,
ROADWAY_BUILDING,
DOOR_OPENED,
DOOR_CLOSED,
LADDER,
CRAWL,
CROUCH,
FENCE_WALL,
JUMP
}
class PGFilter : Managed
{
// Uses PGPolyFlags bitmasks
proto native int GetIncludeFlags();
proto native int GetExcludeFlags();
proto native int GetExlusiveFlags();
proto native void SetFlags(int includeFlags, int excludeFlags, int exclusiveFlags);
proto native void SetCost(PGAreaType areaType, float cost);
}
class AIWorld : Managed
{
private void AIWorld();
private void ~AIWorld();
proto native AIGroup CreateGroup(string templateName);
proto native AIGroup CreateDefaultGroup();
proto native void DeleteGroup(notnull AIGroup group);
proto native bool FindPath(vector from, vector to, PGFilter pgFilter, out TVectorArray waypoints);
proto native bool RaycastNavMesh(vector from, vector to, PGFilter pgFilter, out vector hitPos, out vector hitNormal);
proto native bool SampleNavmeshPosition(vector position, float maxDistance, PGFilter pgFilter, out vector sampledPosition);
}