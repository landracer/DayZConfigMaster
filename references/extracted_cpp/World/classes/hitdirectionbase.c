// Source: F:/Games/Dayz/scripts/4_world/classes/hitindication/hitdirectionbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
 
class HitDirectionEffectBase
{
    const float DURATION_COEF_MIN = 0.6;
    const float INTENSITY_MIN = 0.6;
    
    float m_HitDirection;
    float m_Duration;
    float m_BreakPoint;
    float m_TimeActive;
    float m_IntensityMax;
    
    Widget m_LayoutRoot;
    Widget m_Image;
    
    DayZPlayer m_Player;
    
    bool m_Initialized;
    int m_SizeXEnf;
    int m_SizeYEnf;
    float m_PosX;
    float m_PosY;
    float m_PosXScreenEdge;
    float m_PosYScreenEdge;
    float m_AngleRad;
    float m_AngleRadPrev;
    float m_SmoothVel[1];
    
    ref HitDirectionImagesBase m_ImageData;
    
    void HitDirectionEffectBase()
    {
        m_Initialized = false;
        m_PosX = 0.0;
        m_PosY = 0.0;
        m_AngleRad = 0.0;
        m_SmoothVel[0] = 0.0;
        
        m_ImageData = GetImageData();
        m_ImageData.GetCurrentImageData(m_LayoutRoot,m_Image);
