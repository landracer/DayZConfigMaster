// ======================================================================
// File: dayzphysics_8c_source.html
// Category: other
// ======================================================================

enum PhxInteractionLayers
{
NOCOLLISION,
DEFAULT,
BUILDING,
CHARACTER,
VEHICLE,
DYNAMICITEM,
DYNAMICITEM_NOCHAR,
ROADWAY,
VEHICLE_NOTERRAIN,
CHARACTER_NO_GRAVITY,
RAGDOLL_NO_CHARACTER,
FIREGEOM,
DOOR,
RAGDOLL,
WATERLAYER,
TERRAIN,
GHOST,
WORLDBOUNDS,
FENCE,
AI,
AI_NO_COLLISION,
AI_COMPLEX,
TINYCAPSULE,
TRIGGER,
TRIGGER_NOTERRAIN,
ITEM_SMALL,
ITEM_LARGE,
CAMERA,
TEMP
};
class RaycastRVParams
{
vector begPos;
vector endPos;
Object ignore;
Object with;
float radius;
CollisionFlags flags;
int type;
bool sorted;
bool groundOnly;
void RaycastRVParams( vector vBeg, vector vEnd, Object pIgnore = null, float fRadius = 0.0 )
{
begPos = vBeg;
endPos = vEnd;
ignore = pIgnore;
radius = fRadius;
// default values
with = null;
flags = CollisionFlags.NEARESTCONTACT;
type = ObjIntersectView;
sorted = false;
groundOnly = false;
}
};
class RaycastRVResult
{
Object obj;
Object parent;
vector pos;
vector dir;
int hierLevel;
int component;
SurfaceInfo surface;
bool entry;
bool exit;
};
class CollisionOverlapCallback : Managed
{
bool OnContact(IEntity other, Contact contact)
{
return true;
}
};
class DayZPhysics
{
private void DayZPhysics() {}
private void ~DayZPhysics() {}
proto static bool RaycastRV(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, /*out*/ set<Object> results = NULL, Object with = NULL, Object ignore = NULL, bool sorted = false, bool ground_only = false, int iType = ObjIntersectView, float radius = 0.0, CollisionFlags flags = CollisionFlags.NEARESTCONTACT);
//I am so sorry about this, I am unable to change RaycastRV above without breaking rest of DZ
//proto static bool RaycastRVExt(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, /*out*/ array<string> resultSurfaces = NULL, /*out*/ array<Object> resultObjects = NULL, Object with = NULL, Object ignore = NULL, bool sorted = false, bool ground_only = false, int iType = ObjIntersectView, float radius = 0.0, CollisionFlags flags = CollisionFlags.NEARESTCONTACT);
proto static bool GetHitSurface(Object other, vector begPos, vector endPos, string surface);
proto static bool GetHitSurfaceAndLiquid(Object other, vector begPos, vector endPos, string surface, out int liquidType);
proto static bool RaycastRVProxy( notnull RaycastRVParams in, out notnull array< ref RaycastRVResult> results, array< Object > excluded = null );
proto static bool RayCastBullet(vector begPos, vector endPos, PhxInteractionLayers layerMask, Object ignoreObj, out Object hitObject, out vector hitPosition, out vector hitNormal, out float hitFraction);
proto static bool SphereCastBullet(vector begPos, vector endPos, float radius, PhxInteractionLayers layerMask, Object ignoreObj, out Object hitObject, out vector hitPosition, out vector hitNormal, out float hitFraction);
proto static bool GeometryOverlapBullet(vector transform[4], dGeom geometry, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool EntityOverlapBullet(vector transform[4], IEntity entity, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool EntityOverlapSingleBullet(vector transform[4], IEntity entity, IEntity other, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool SphereOverlapBullet(vector position, float radius, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool CylinderOverlapBullet(vector transform[4], vector extents, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool CapsuleOverlapBullet(vector transform[4], float radius, float height, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
proto static bool BoxOverlapBullet(vector transform[4], vector extents, PhxInteractionLayers layerMask, notnull CollisionOverlapCallback callback);
}