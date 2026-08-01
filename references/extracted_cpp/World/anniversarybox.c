// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/container_base/anniversarybox.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnniversaryBox extends Container_Base
{
    private const int SPAWN_SHIRTS_MAX = 60;
    
    #ifndef SERVER
    protected AnniversaryBoxLight m_Light;
    #endif
    
    override void DeferredInit()
    {       
        super.DeferredInit();       
        
        #ifndef SERVER
        m_Light = AnniversaryBoxLight.Cast(ScriptedLightBase.CreateLight(AnniversaryBoxLight, &quot;0 0 0&quot;));
        if (m_Light)
            m_Light.AttachOnMemoryPoint(this, &quot;light&quot;);
        #endif
                
