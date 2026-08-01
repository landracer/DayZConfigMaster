// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionattachontrap.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionAttachOnTrap: ActionAttach
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && !target.GetObject().IsInherited(TrapBase))
        {
            return false;
        }
        
        return super.ActionCondition(player, target, item);
