// Source: F:/Games/Dayz/scripts/5_mission/mission/gameplayeffectwidgets/bleedingindicators/bleedingdrop.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class BleedingIndicatorDropData
{
    protected ImageWidget   m_Widget;
    protected int           m_Severity;
    protected float         m_TimeTotal;
    protected float         m_ProgressBreakpointTime;
    protected float         m_ProgressFadingDuration; //remaining duration AFTER breakpoint
    protected float         m_ProgressBreakpoint;
    protected float         m_Duration;
    protected float         m_SpeedCoef;
    protected int           m_ScatterPx;
    protected float         m_SlideDistance;
    
    protected float         m_ColorAlphaStart;
    protected float         m_ColorAlphaEnd;
    protected float         m_ColorAlphaCurrent;
    
    protected float         m_ImageBaseSizeX;
    protected float         m_ImageBaseSizeY;
    protected float         m_ImageStartingSizeX; //adjusted by percentage
    protected float         m_ImageStartingSizeY; //adjusted by percentage
    protected float         m_ImageEndSizeX; //adjusted by percentage
    protected float         m_ImageEndSizeY; //adjusted by percentage
    protected float         m_ImageMaxSizeX;
    protected float         m_ImageMaxSizeY;
    protected float         m_ImageBaseRotation;
    protected bool          m_IsRunning;
    protected vector        m_BasePosition;
    protected int           m_ScreenSizeX;
    protected int           m_ScreenSizeY;
    protected float         m_PosX, m_PosY;
    protected float         m_StartSizeCoef;
    protected float         m_EndSizeCoef;
    protected float         m_RandomSizeMin;
    protected float         m_RandomSizeMax;
    
    //Written with relative positioning in mind
    void BleedingIndicatorDropData(ImageWidget image, int severity)
    {
        m_Widget = image;
        m_Severity = severity;
        m_TimeTotal = 0;
        m_IsRunning = false;
        
#ifdef DIAG_DEVELOPER
        if (DbgBleedingIndicationStaticInfo.m_DbgUseOverrideValues)
        {
            m_Duration = DbgBleedingIndicationStaticInfo.m_DbgDropDurationBase;
            m_ProgressBreakpointTime = m_Duration * DbgBleedingIndicationStaticInfo.m_DbgDropProgressTreshold;
            m_ProgressBreakpoint = DbgBleedingIndicationStaticInfo.m_DbgDropProgressTreshold;
            m_StartSizeCoef = DbgBleedingIndicationStaticInfo.m_DbgDropStartSize;
            m_EndSizeCoef = DbgBleedingIndicationStaticInfo.m_DbgDropEndSize;
            m_RandomSizeMin = DbgBleedingIndicationStaticInfo.m_DbgDropSizeVariationMin;
            m_RandomSizeMax = DbgBleedingIndicationStaticInfo.m_DbgDropSizeVariationMax;
            m_ScatterPx = DbgBleedingIndicationStaticInfo.m_DbgDropScatter;
            m_SlideDistance = DbgBleedingIndicationStaticInfo.m_DbgDropSlideDistance;
        }
        else
#endif
        {
            switch (m_Severity)
            {
                case BleedingIndicationConstants.INDICATOR_SEVERITY_LOW:
                {
                    m_Duration = BleedingIndicationConstants.DROP_DURATION_LOW;
                    m_StartSizeCoef = BleedingIndicationConstants.DROP_SIZE_START_LOW;
                    m_EndSizeCoef = BleedingIndicationConstants.DROP_SIZE_END_LOW;
                    m_RandomSizeMin = BleedingIndicationConstants.DROP_SIZE_VARIATION_MIN_LOW;
                    m_RandomSizeMax = BleedingIndicationConstants.DROP_SIZE_VARIATION_MAX_LOW;
                    m_ScatterPx = BleedingIndicationConstants.DROP_SCATTER_LOW;
                    m_SlideDistance = BleedingIndicationConstants.DROP_SLIDE_DISTANCE_LOW;
                    break;
                }
                case BleedingIndicationConstants.INDICATOR_SEVERITY_MEDIUM:
                {
                    m_Duration = BleedingIndicationConstants.DROP_DURATION_MEDIUM;
                    m_StartSizeCoef = BleedingIndicationConstants.DROP_SIZE_START_MEDIUM;
                    m_EndSizeCoef = BleedingIndicationConstants.DROP_SIZE_END_MEDIUM;
                    m_RandomSizeMin = BleedingIndicationConstants.DROP_SIZE_VARIATION_MIN_MEDIUM;
                    m_RandomSizeMax = BleedingIndicationConstants.DROP_SIZE_VARIATION_MAX_MEDIUM;
                    m_ScatterPx = BleedingIndicationConstants.DROP_SCATTER_MEDIUM;
                    m_SlideDistance = BleedingIndicationConstants.DROP_SLIDE_DISTANCE_MEDIUM;
                    break;
                }
                case BleedingIndicationConstants.INDICATOR_SEVERITY_HIGH:
                {
                    m_Duration = BleedingIndicationConstants.DROP_DURATION_HIGH;
                    m_StartSizeCoef = BleedingIndicationConstants.DROP_SIZE_START_HIGH;
                    m_EndSizeCoef = BleedingIndicationConstants.DROP_SIZE_END_HIGH;
                    m_RandomSizeMin = BleedingIndicationConstants.DROP_SIZE_VARIATION_MIN_HIGH;
                    m_RandomSizeMax = BleedingIndicationConstants.DROP_SIZE_VARIATION_MAX_HIGH;
                    m_ScatterPx = BleedingIndicationConstants.DROP_SCATTER_HIGH;
                    m_SlideDistance = BleedingIndicationConstants.DROP_SLIDE_DISTANCE_HIGH;
                    break;
                }
            }
            
            m_ProgressBreakpointTime = m_Duration * BleedingIndicationConstants.DROP_PROGRESS_THRESHOLD;
            m_ProgressBreakpoint = BleedingIndicationConstants.DROP_PROGRESS_THRESHOLD;
        }
        m_ProgressFadingDuration = Math.Max(0.0001, m_Duration - m_ProgressBreakpointTime);
        
        m_SpeedCoef = 1.0; //TODO ??
#ifdef DIAG_DEVELOPER
        if (DbgBleedingIndicationStaticInfo.m_DbgUseOverrideValues)
        {
            m_ColorAlphaStart = DbgBleedingIndicationStaticInfo.m_DbgDropColorAlphaStart / 255;
            m_ColorAlphaEnd = DbgBleedingIndicationStaticInfo.m_DbgDropColorAlphaEnd / 255;
        }
        else
#endif
        {
            m_ColorAlphaStart = BleedingIndicationConstants.DROP_COLOR_ALPHA_START / 255;
            m_ColorAlphaEnd = BleedingIndicationConstants.DROP_COLOR_ALPHA_END / 255;
        }
        InitImageScale();
        GetScreenSize(m_ScreenSizeX,m_ScreenSizeY);
        AdjustColorSaturation();
