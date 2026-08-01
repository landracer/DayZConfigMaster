// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/tentbase/shelter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShelterBase extends TentBase
{
    static const string SITE_OBJECT_TYPE = &quot;ShelterSite&quot;;
 
    void ShelterBase()
    {
        m_ShowAnimationsWhenPitched.Insert(&quot;Body&quot;);
        m_ShowAnimationsWhenPacked.Insert(&quot;Body&quot;);
        
        m_HalfExtents = Vector(0.8, 0.15, 1.3);
