// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/platecarriervest.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlateCarrierVest: Vest_Base
{
    override void OnWasAttached( EntityAI parent, int slot_id )
    {
        super.OnWasAttached( parent, slot_id );
        
        if ( g_Game.IsServer() && parent.IsInherited( DayZInfected ) )
        {
            float coef = Math.RandomFloatInclusive( 0.2, 0.4 );
            SetHealth01( &quot;&quot;, &quot;&quot;, coef);
        }
