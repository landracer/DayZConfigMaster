// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/rangefinder.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Rangefinder extends PoweredOptic_Base
{
    static const float RANGEFINDER_MAX_DISTANCE = 913.4856; //TODO adjust maximal distance to match real life rangefinder
    
    protected ref Timer                 m_Timer;
    protected Widget                    m_Root;
    protected TextWidget                m_RangeText;
    
    protected string                    m_LayoutPath;
    
    void Rangefinder()
    {
        InitRangeFinderData();
