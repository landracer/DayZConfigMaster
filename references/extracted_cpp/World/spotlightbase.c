// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SpotLightBase extends ScriptedLightBase
{
    void SpotLightBase()
    {
        SetLightType(LightSourceType.SpotLight); // This function must be called in constructor of the light!
