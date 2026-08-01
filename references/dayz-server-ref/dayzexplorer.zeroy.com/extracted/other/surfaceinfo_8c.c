// ======================================================================
// File: surfaceinfo_8c_source.html
// Category: other
// ======================================================================

typedef int[] SurfaceInfo;
class SurfaceInfo : SurfaceProperties
{
protected void SurfaceInfo() {};
protected void ~SurfaceInfo() {};
proto static SurfaceInfo GetByName(string name);
proto static SurfaceInfo GetByFile(string name);
proto string GetName();
proto string GetEntryName();
proto string GetSurfaceType();
proto float GetRoughness();
proto float GetDustness();
proto float GetBulletPenetrability();
proto float GetThickness();
proto float GetDeflection();
proto float GetTransparency();
proto float GetAudability();
proto bool IsLiquid();
proto bool IsStairs();
proto bool IsPassthrough();
proto bool IsSolid();
proto string GetSoundEnv();
proto string GetImpact();
proto int GetLiquidType();
proto int GetStepParticleId();
proto int GetWheelParticleId();
};
enum UseObjectsMode
{
Wait,
NoWait,
NoLock
};
enum SurfaceDetectionType
{
Scenery,
Roadway
};
/*sealed*/ class SurfaceDetectionParameters
{
SurfaceDetectionType type = SurfaceDetectionType.Scenery;
vector position;
bool includeWater = false;
UseObjectsMode syncMode = UseObjectsMode.Wait;
Object ignore = null;
RoadSurfaceDetection rsd = RoadSurfaceDetection.ABOVE;
};
/*sealed*/ class SurfaceDetectionResult
{
float height = 0;
float normalX = 0;
//float normalY = 1;
float normalZ = 0;
SurfaceInfo surface = null;
bool aboveWater = false;
Object object = null;
};