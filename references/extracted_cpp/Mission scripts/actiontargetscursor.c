// Source: F:/Games/Dayz/scripts/5_mission/gui/actiontargetscursor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ATCCachedObject
{
    protected Object    m_CachedObject;
    protected vector    m_CursorWPos;
    protected vector    m_ScreenPos;
    protected int       m_CompIdx;
    
    void ATCCachedTarget()
    {
        m_CachedObject = null;
        m_ScreenPos = vector.Zero;
        m_CompIdx = -1;
