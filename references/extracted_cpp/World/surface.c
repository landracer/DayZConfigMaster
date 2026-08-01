// Source: F:/Games/Dayz/scripts/4_world/static/surface.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Surface
{
    static int GetStepsParticleID(string surface_name) 
    {
        return SurfaceInfo.GetByName(surface_name).GetStepParticleId();
