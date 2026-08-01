// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PointLightBase extends ScriptedLightBase
{
    void PointLightBase()
    {
        SetLightType(LightSourceType.PointLight); // This function must be called in constructor of the light!
