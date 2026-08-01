// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/trigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class TriggerInsider
{
    ref OLinkT insider; // DEPRECATED
    
    protected Object m_Object;
    
    int timeStamp;
    
    float timeEntered;
    
    float lastUpdated;
    
    void TriggerInsider(Object obj)
    {
        insider = new OLinkT(obj);
        m_Object = obj;
