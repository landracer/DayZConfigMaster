// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/damagecomponents/areadamagecomponentraycasted.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AreaDamageComponentRaycasted : AreaDamageComponent
{
    // Defined in local space of the trigger
    ref array<vector>               m_RaycastSources;
    vector                          m_RaycastEndOffset;
    
    ref array<ref RaycastRVResult>  m_RaycastCache;
    int                             m_RaycastCachedIndex;
    
    void AreaDamageComponentRaycasted(AreaDamageManager parent)
    {
        m_RaycastSources = new array<vector>;
        m_RaycastEndOffset = &quot;0 0.5 0&quot;;
        
        m_RaycastCache = new array<ref RaycastRVResult>;
        m_RaycastCachedIndex = -1;
