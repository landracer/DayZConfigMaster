// Source: F:/Games/Dayz/scripts/5_mission/gui/crosshairselector.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CrossHair
{
    protected string                    m_Name;
    protected bool                      m_Shown;
    protected bool                      m_Current;
 
    protected Widget                    m_Widget;
    
    void CrossHair(Widget w)
    {
        m_Widget = w;
        m_Name = w.GetName();
        m_Shown = true;
        m_Current = false;
