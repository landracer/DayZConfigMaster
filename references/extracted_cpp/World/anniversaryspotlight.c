// Source: F:/Games/Dayz/scripts/4_world/entities/building/specific/anniversaryspotlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnniversarySpotLight : Building
{
    #ifndef SERVER
    protected AnniversaryMainLight m_Light; 
    #endif
 
    protected bool m_LightState;
    
    void AnniversarySpotLight()
    {
        RegisterNetSyncVariableBool(&quot;m_LightState&quot;);
