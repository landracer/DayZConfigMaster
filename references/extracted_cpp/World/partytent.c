// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/tentbase/partytent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PartyTent extends TentBase
{
    void PartyTent()
    {
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door1o&quot;, &quot;Door1c&quot;, OPENING_0), 0 );
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door2o&quot;, &quot;Door2c&quot;, OPENING_1), 0 );
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door3o&quot;, &quot;Door3c&quot;, OPENING_2), 0 );
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door4o&quot;, &quot;Door4c&quot;, OPENING_3), 0 );
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door5o&quot;, &quot;Door5c&quot;, OPENING_4), 0 );
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;Door6o&quot;, &quot;Door6c&quot;, OPENING_5), 0 );
        
        m_ShowAnimationsWhenPitched.Insert( &quot;Body&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Pack&quot; );
        /*m_ShowAnimationsWhenPitched.Insert( &quot;Door1o&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Door2o&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Door3o&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Door4o&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Door5o&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Door6o&quot; );*/
        
        m_ShowAnimationsWhenPacked.Insert( &quot;Inventory&quot; );
        
        m_HalfExtents = Vector(1.3, 0.35, 2.7);
