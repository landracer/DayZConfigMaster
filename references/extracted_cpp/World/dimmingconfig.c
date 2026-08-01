// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/dimming/dimmingconfig.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DimmingConfig
{
    //randomization
    float m_DimBrigthnessMin = 0.1;
    float m_DimBrigthnessMax = 1;
    float m_DimSpeedMin = 0.1;
    float m_DimSpeedMax = 1;
    float m_DimBrigthnessDurationMin = 0.1;
    float m_DimBrigthnessDurationMax = 1;
    float m_BrightenSpeedMin = 0.1;
    float m_BrightenSpeedMax = 1;
    float m_DimBrigthnessPauseMin = 1;
    float m_DimBrigthnessPauseMax = 4;
    
    // patterns
    bool    m_UsePatterns;
    int     m_PatternRepeatCount = -1; // -1 = infinite
    int     m_CurrentPatternID;
    int     m_PatternCount;
    int     m_CurrentEntryID;
    ref array<ref array<int>>   m_BrightnessPatterns = new array<ref array<int>>();
    ref array<ref array<float>> m_DimSpeedPatterns = new array<ref array<float>>();
    ref array<ref array<float>> m_DurationPatterns = new array<ref array<float>>();
    ref array<ref array<float>> m_BrightenSpeedPatterns = new array<ref array<float>>();
    ref array<ref array<float>> m_PausePatterns = new array<ref array<float>>();
    
    // Percentage of base brightness, how strong can the dim be 
    void SetRandomBrigthnessLimits(float min, float max)
    {
        min = Math.Clamp(min, 0, 100);
        max = Math.Clamp(max, 0, 100);
        
        m_DimBrigthnessMin = min * 0.01;
        m_DimBrigthnessMax = max * 0.01;
