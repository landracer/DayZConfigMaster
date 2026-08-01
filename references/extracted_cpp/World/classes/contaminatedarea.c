// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/contaminatedarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContaminatedArea_Base : EffectArea
{
    override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
    {
        super.OnPlayerEnterServer(player, trigger);
 
        player.IncreaseContaminatedAreaCount();
