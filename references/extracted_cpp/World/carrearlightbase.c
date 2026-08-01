// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/carlightbase/carrearlightbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CarRearLightBase extends CarLightBase
{
    // Brake light only. These are just some default values which are overwritten down the hierarchy.
    float   m_SegregatedBrakeBrightness = 2;
    float   m_SegregatedBrakeRadius = 6;
    float   m_SegregatedBrakeAngle = 180;
    vector  m_SegregatedBrakeColorRGB = Vector(1, 0.05, 0.05);
    
    void CarRearLight()
    {
        // These are just some default values which can be overwrittend down in the hierarchy.
        // Reverse light only
        m_SegregatedBrightness = 2;
        m_SegregatedRadius = 10;
        m_SegregatedAngle = 180;
        m_SegregatedColorRGB = Vector(1.0, 1.0, 1.0);
        
        // Brake & Revese lights combined
        m_AggregatedBrightness = 2.5;
        m_AggregatedRadius = 12;
        m_AggregatedAngle = 180;
        m_AggregatedColorRGB = Vector(1.0, 0.5, 0.5);
        
        FadeIn(0.1);
        SetFadeOutTime(0.1);
        SetVisibleDuringDaylight(false);
        SetCastShadow(false);
        SetFlareVisible(false);
        EnableSpecular(false); // Specular is disabled because it causes ugly shining on cars
        
        SegregateLight();
