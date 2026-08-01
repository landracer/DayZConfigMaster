// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
Please remember that:
-Lights work only on client side!
-Lights with Brightness or Radius of 0 (or less) are automatically deleted
-Lights are very performance heavy. Especially if they cast shadows. Use them carefully!
 
Script author: Boris Vacula
*/
 
class ScriptedLightBase extends EntityLightSource
{
    float       m_LifetimeStart;
    float       m_LifetimeEnd = -1; // -1 makes this light permanent
    float       m_FadeOutTime = -1;
    float       m_FadeInTime = -1;
    float       m_Radius;
    float       m_RadiusTarget;
    float       m_Brightness;
    float       m_BrightnessPulse; // flicker effect
    float       m_BrightnessPulseSpeed;
    float       m_BrightnessPulseAmplitudeMax;
    float       m_BrightnessPulseAmplitudeMin;
    float       m_BrightnessTarget;
    float       m_BrightnessSpeedOfChange = 1;
    float       m_RadiusSpeedOfChange = 1;
    float       m_OptimizeShadowsRadius = 0; // Within this range between the light source and camera the shadows will be automatically disabled to save on performance
    
    float       m_DancingShadowsAmplitude;
    float       m_DancingShadowsSpeed;
    
    float       m_BlinkingSpeed;
    protected int m_HiddenSelectionID;
    
    bool        m_IsDebugEnabled = false;
    
    Object      m_Parent; // Attachment parent
    vector      m_LocalPos; // Local position to my attachment parent
    vector      m_LocalOri; // Local orientation to my attachment parent
    vector      m_DancingShadowsLocalPos;
        
    ref Timer   m_DeleteTimer;
    protected ref LightDimming m_LightDimming;
    
    static ref set<ScriptedLightBase> m_NightTimeOnlyLights = new set<ScriptedLightBase>();
    
    void ScriptedLightBase()
    {
        m_LifetimeStart = g_Game.GetTime();
        SetEnabled(true);
        SetEventMask(EntityEvent.FRAME);
        SetEventMask(EntityEvent.INIT);
