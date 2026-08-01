// Source: F:/Games/Dayz/scripts/3_game/global/world.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class World: Managed
{
    // exposed from native, do not touch
    #ifdef DIAG_DEVELOPER
    const int DM_GAME_WORLD_SIMUL_ACCELERATION;
    #endif
 
    //proto private void ~World();
    //proto private void World();
 
    proto void CheckSoundObstruction(EntityAI source,  bool inSource, out float obstruction, out float occlusion);
 
    proto native void   GetPlayerList(out array<Man> players);

    proto native void SetTimeMultiplier(float timeMultiplier);
    
    proto void          GetDate(out int year, out int month, out int day, out int hour, out int minute);
    
    proto native void   SetDate(int year, int month, int day, int hour, int minute);
    proto native float  GetLatitude();
    proto native float  GetLongitude();
    proto native float  GetMoonIntensity();
    proto native float  GetSunOrMoon();
    proto native bool   IsNight();
        
    proto native float  GetEyeAccom();
    proto native void   SetEyeAccom(float eyeAccom);
 
    proto native void   StartFpsBenchmark(int sectorsCount, float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue, float minX, float minY, float maxX, float maxY);
    proto native void   StartFpsBenchmarkWithMask(float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue);
    proto native void   StartFpsBenchmarkViews(string xmlFileName, float preloadDistance);
    proto native void   SetBuldozerWaterEnabled(bool enable);
    proto native void SetPreferredViewDistance(float distance);
 
    proto native void SetViewDistance(float distance);
 
    proto native void SetObjectViewDistance(float distance);
    
    proto native void SetExplicitVolumeFactor_EnvSounds2D(float factor, float fadeTime);
    
          
    proto native void AddEnvShootingSource(vector position, float shootingValDecrease);
    
    proto int GetWorldSize();
  
    proto void GetGridCoords(vector pos, float gridSize, out int gridX, out int gridZ);

    proto native void LoadNewLightingCfg(string path);

    proto native void LoadUserLightingCfg(string path, string name);

    proto native void SetUserLightingLerp(float val);
 
      
    proto native AIWorld GetAIWorld();
      
    proto native void UpdatePathgraphDoorByAnimationSourceName(notnull Object object, string animSourceName);
    
    proto native void MarkObjectForPathgraphUpdate(Object object);
    proto native void ProcessMarkedObjectsForPathgraphUpdate();
 
    
    proto native Material GetMaterial(string materialName);

    proto native void       SetCameraPostProcessEffect(int cam, int ppEffect, string effectName, string materialName);
    
    void SetAperture(float invDiameter)
    {
        if (invDiameter <= 0)
        {
            SetEyeAccom(-1);
        }
        else
        {
            float apertureArea = 1.0 / invDiameter;
            SetEyeAccom(apertureArea * apertureArea);
        }
