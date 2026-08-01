// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedboats/boat_01.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Boat_01_ColorBase : BoatScript
{
    override void EEInit()
    {       
        super.EEInit();
 
        SetAnimationPhase(&quot;ShowDamage&quot;,0);
        SetAnimationPhase(&quot;HideDamage&quot;,1);
