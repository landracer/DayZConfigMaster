// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/tentbase/cartent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CarTent extends TentBase
{
    void CarTent()
    {       
        m_ToggleAnimations.Insert( new ToggleAnimations(&quot;EntranceO&quot;, &quot;EntranceC&quot;, OPENING_0), 0 );
            
        m_ShowAnimationsWhenPitched.Insert( &quot;Body&quot; );
        //m_ShowAnimationsWhenPitched.Insert( &quot;EntranceO&quot; );
        m_ShowAnimationsWhenPitched.Insert( &quot;Pack&quot; );
        
        m_ShowAnimationsWhenPacked.Insert( &quot;Inventory&quot; );
        
        m_HalfExtents = Vector(1.8, 0.33, 3.4);
