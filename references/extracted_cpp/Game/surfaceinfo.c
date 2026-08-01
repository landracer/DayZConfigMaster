// Source: F:/Games/Dayz/scripts/3_game/surfaceinfo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

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
