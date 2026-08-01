// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actiontargets.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class VicinityObjects
{
    private ref map<Object, Object> m_VicinityObjects;
    
    void VicinityObjects()
    {
        m_VicinityObjects = new map<Object, Object>;
