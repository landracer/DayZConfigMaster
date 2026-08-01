// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/carlightbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CarLightBase extends SpotLightBase
{
    // What follows are just default light parameters which are overwriten down in the hierarchy in constructors.
    // &#39;Segregated&#39; means value for a light source that consists of 1 bulb (ex.: when 1 of 2 headlights are destroyed)
    // &#39;Aggregated&#39; means value for 1 light source that consists of 2 bulbs (ex.: when both headlights are shining, or brake & reverse lights are shining at once)
    // Aggregating lights saves a lot of performance
    
    float       m_SegregatedBrightness = 20;
    float       m_SegregatedRadius = 66;
    float       m_SegregatedAngle = 90;
    vector      m_SegregatedColorRGB = Vector(1.0, 0.8, 0.6);
    
    float       m_AggregatedBrightness = 30;
    float       m_AggregatedRadius = 100;
    float       m_AggregatedAngle = 100;
    vector      m_AggregatedColorRGB = Vector(1.0, 0.8, 0.6);
    
    void CarLightBase()
    {
        SetVisibleDuringDaylight(true);
        SetCastShadow(true);
        SetFlareVisible(true);
        FadeIn(0.3);
        SetFadeOutTime(0.25);
        
