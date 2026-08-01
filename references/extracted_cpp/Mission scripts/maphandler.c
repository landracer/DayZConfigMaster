// Source: F:/Games/Dayz/scripts/5_mission/gui/maphandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MapHandler : ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    
    void MapHandler(Widget w)
    {
        m_Root = w;
        m_Root.SetHandler(this);
