// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/contaminatedtrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// In this file you will find both Static and Dynamic contaminated area triggers
class ContaminatedTrigger extends EffectTrigger
{
    const float DAMAGE_TICK_RATE = 10;//deal damage every n-th second
    
    override string GetDisplayName()
    {
        return &quot;#STR_contaminatedarea&quot;;
