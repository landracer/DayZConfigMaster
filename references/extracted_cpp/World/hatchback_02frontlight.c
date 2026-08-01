// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/carlightbase/hatchback_02frontlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hatchback_02FrontLight extends CarLightBase
{
    void Hatchback_02FrontLight()
    {
        m_SegregatedBrightness = 6;
        m_SegregatedRadius = 65;
        m_SegregatedAngle = 80;
        m_SegregatedColorRGB = Vector(0.8, 0.8, 1);
        
        m_AggregatedBrightness = 12;
        m_AggregatedRadius = 90;
        m_AggregatedAngle = 90;
        m_AggregatedColorRGB = Vector(0.8, 0.8, 1);
        
        FadeIn(0.3);
        SetFadeOutTime(0.25);
        
        SegregateLight();
