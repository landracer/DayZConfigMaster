// Source: F:/Games/Dayz/scripts/4_world/classes/mousebuttoninfo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MouseButtonInfo
{
    private int     m_ButtonID;
    private int     m_TimeLastPress;
    private int     m_TimeLastRelease;
    
    void MouseButtonInfo(int button)
    {
        m_ButtonID          = button;
        m_TimeLastPress     = -1;
        m_TimeLastRelease   = -1;
