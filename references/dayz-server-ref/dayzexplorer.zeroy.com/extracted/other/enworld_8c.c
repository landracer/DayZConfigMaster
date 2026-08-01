// ======================================================================
// File: enworld_8c_source.html
// Category: other
// ======================================================================

//----------------------------------------------
typedef int[] WorldHandle;
proto native float GetWorldTime();
proto native WorldHandle SetCurrentWorld(WorldHandle world);
//proto native void SchedulePreload(vector pos, float radius);
proto native IEntity FindEntityByName(IEntity worldEnt, string name);
proto native IEntity FindEntityByID(IEntity worldEnt, int ID);
proto native int GetNumActiveEntities(IEntity worldEntity);
proto native IEntity GetActiveEntity(IEntity worldEntity, int index);
//----------------------------------------------
enum CameraType
{
PERSPECTIVE,
ORTHOGRAPHIC
};
proto native void SetListenerCamera(int camera);
proto native void SetCamera(int cam, vector origin, vector angle);
proto native void SetCameraEx(int cam, const vector mat[4]);
proto native void GetCamera(int cam, out vector mat[4]);
proto native void SetCameraVerticalFOV(int cam, float fovy);
proto native void SetCameraFarPlane(int cam, float farplane); //default 160000 units
proto native void SetCameraNearPlane(int cam, float nearplane); //default 5 units
proto native void SetCameraType(int cam, CameraType type);
enum PostProcessEffectType
{
None,
UnderWater,
SSAO,
DepthOfField,
HBAO,
RotBlur,
GodRays,
Rain,
Snowfall,
FilmGrain,
RadialBlur,
ChromAber,
WetDistort,
DynamicBlur,
ColorGrading,
Colors,
Glow,
SMAA,
FXAA,
Median,//unused?
SunMask,
GaussFilter,
SSR, //not available
Distort,
Ghost
};
proto native void SetCameraPostProcessEffect(int cam, int priority, PostProcessEffectType type, string materialPath);
//ent can be NULL for world-space coords
proto vector ProjectVector(int cam, IEntity ent, vector vec);
proto vector UnprojectVector(int cam, float x, float y, vector dir);
//----------------------------------------------
typedef int[] HLIGHT;
enum LightType
{
POINT, //< point light, all directional light
SPOT, //< spot light, direction is determined by owner (entity)
DIRECTIONAL,
AMBIENT
};
enum LightFlags
{
DYNAMIC,
CASTSHADOW,
CHEAP
};
proto HLIGHT AddLight(IEntity owner, LightType type, LightFlags flags, float radius, vector color);
proto native bool RemoveLight(HLIGHT light);
proto native bool SetLightEx(HLIGHT light, float radius, vector color);
proto native bool SetLightTexture(HLIGHT light, string cubemap);
proto native int SetLightFlags(HLIGHT light, LightFlags flags);
proto native int ClearLightFlags(HLIGHT light, LightFlags flags);
proto native bool SetLightCone(HLIGHT light, float cone);
proto native float GetSceneHDRMul(int camera);
//----------------------------------------------
enum TraceFlags
{
BONES, //< tests collision geometries around bones of animated objects
ENTS, //< tests entities
WORLD, //< tests world bounding box
ONLY_PHYSICS,
WATER, //< tests collision with water surface
PASSTRANSLUCENT,//< Do not intersects with entities with EntityFlags.TRANSLUCENT set
RAGDOLLS, //< tests ragdolls
VISTEST, //< performs visibility test first. Not necessary for entities receiving EntityEvent.VISIBLE, because there is a certainty that a camera will see them
NOTRACE,
TRANSPARENT_OCCLUDERS
};
enum TraceShape
{
LINE,
BOX,
OBB,
SPHERE
};
class TraceContact
{
float Fraction;
int Content;
int Surfparm;
int MaterialFlags;
int Triangle;
int SurfaceID;
owned string MaterialName;
owned string OriginalMaterialName;
float Plane[4];
vector Point;
}
proto native bool TraceLineToEntity(IEntity ent, vector start, vector end, out TraceContact contact);
//bool FilterCallback(Class target [, vector rayorigin, vector raydirection])
class TraceParam: Managed
{
vector Start;
vector End;
int LayerMask = 0xffffffff;
TraceFlags Flags;
IEntity Exclude;
};
class TraceSphere: TraceParam
{
float Radius;
};
class TraceBox: TraceParam
{
vector Mins;
vector Maxs;
};
class TraceOBB: TraceBox
{
vector Mat[3];
};
proto volatile float TraceMove(TraceParam param, out IEntity cent, out float plane[4], out int surfparm, func filtercallback);
proto native int P2PVisibilityEx(vector from, vector to,int flags);
proto int SphereQuery(vector origin, float radius, out IEntity visents[], int ents, int fmask);
proto native bool IsBoxVisible(vector mins, vector maxs, int flags);
proto int VisEntities(vector origin, vector look, float angle, float radius, out IEntity ents[2], int maxents, int fmask);
class OcclusionQuery
{
proto private void ~OcclusionQuery();
proto native int GetResult();
proto native void SetPosition(vector pos);
proto native void Destroy();
};
//----------------------------------------------
typedef int[] hDecal;
proto native hDecal CreateDecal(IEntity entity, vector origin, vector project, float nearclip, float angle, float size, string materialName, float lifetime, int flags);
proto native void RemoveDecal(hDecal decal);
proto native hDecal CreateLandMarkDecal(IEntity entity, vector origin, vector normal, float edgeSize, float lifeTime, string materialName, hDecal prevDecal, float alpha);
proto native int CanAddToLandMarkDecal(hDecal lmDecal, IEntity entity, string mat, vector newPoint);
proto native bool AddPointToLandMarkDecal(hDecal lmDecal, vector point, vector normal, float alpha);
proto native void FinalizeLandMarkDecal(hDecal lmDecal, bool addAlpha, float alphaDist);
proto native bool IsLandMarkFinalized(hDecal lmDecal);
proto native vector GetLastLandMarkPoint(hDecal lmDecal);
proto native void SetGlobalLandMarkParams(float minSegmentLength, float maxSegmentLength, float degAngle);
//----------------------------------------------
proto native bool IsOcean();
proto native float GetOceanHeight(float worldX, float worldZ);
proto native vector GetOceanHeightAndDisplace(float worldX, float worldZ);