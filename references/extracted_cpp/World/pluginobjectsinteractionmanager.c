// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginobjectsinteractionmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginObjectsInteractionManager extends PluginBase
{
    private ref         array<Object> m_LockedObjects;
    private ref         array<float> m_LockedObjectsDecay;
    private const float     TIME_TO_FORCED_UNLOCK = 60;
    private const float     TICK_RATE = 10;
    private ref Timer   m_DecayTimer;
        
    void PluginObjectsInteractionManager()
    {
        m_LockedObjects = new array<Object>;
        m_LockedObjectsDecay = new array<float>;
        //TIMERDEPRECATED - timer for decaying objects 
        m_DecayTimer = new Timer();
        m_DecayTimer.Run(TICK_RATE, this, &quot;Decay&quot;, NULL,true);
