// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperhmp_lvl3.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_HeavyMetalPoisoning_3 extends PPERequester_GameplayBase
{
    const float WAVE_PROGRESS_MIN = 0;
    const float WAVE_PROGRESS_MAX = Math.PI;
    const float WAVE_AMPLITUDE_COEF = 0.7; //utilized values clamped to <-1,1> range, this affects the duration of plateau and steepness of change
    const float WAVE_TILT = -0.12; //anything in the closed range of (1,-1), left to right. UNDEFINED for &#39;0&#39; value!
    
    const float DISTORT_MIN = 0;
    const float DISTORT_MAX = -0.8;
    
    //const float PULSING_FREQUENCY = 1.0;
    
    float m_ProgressAngle;
    float m_CurveValue;
    float m_CurveValueClamped;
    float m_LenseOffsetAngle;
    float m_LenseOffsetDist;
    
    void SetInitialLenseOffset(float offsetAngle, float offsetDistance = 0.5)
    {
        m_LenseOffsetAngle = offsetAngle;
        m_LenseOffsetDist = Math.Clamp(offsetDistance,0,1);
