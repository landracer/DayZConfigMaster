// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/basebuildingbase/totem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TerritoryFlag extends BaseBuildingBase
{
    const float MAX_ACTION_DETECTION_ANGLE_RAD      = 1.3;      //1.3 RAD = ~75 DEG
    const float MAX_ACTION_DETECTION_DISTANCE       = 2.0;      //meters
    
    bool m_RefresherActive;
    bool m_RefresherActiveLocal;
    bool m_RefresherInitialized;
    int m_RefresherTimeRemaining;
    int m_RefreshTimeCounter;
    
    protected int m_FlagRefresherFrequency = GameConstants.REFRESHER_FREQUENCY_DEFAULT;         //how often does the flag increase lifetimes
    protected int m_FlagRefresherMaxDuration = GameConstants.REFRESHER_MAX_DURATION_DEFAULT;    //how long will the refresher run; multiple of m_FlagRefresherFrequency by default
    
    void TerritoryFlag()
    {
        m_RefresherActive = false;
        m_RefresherActiveLocal = false;
        m_RefresherInitialized = false;
        m_RefresherTimeRemaining = 0;
        
        if ( GetCEApi() )
        {
            InitRefresherData();
        }
        RegisterNetSyncVariableBool(&quot;m_RefresherActive&quot;);
