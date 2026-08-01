// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionattachseeds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionAttachSeeds : ActionAttach
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        EntityAI targetEntity = EntityAI.Cast(target.GetObject());
        if (targetEntity && !GardenBase.Cast(targetEntity) && item)
        {
            return targetEntity.GetInventory() && targetEntity.GetInventory().CanAddAttachment(item) && !targetEntity.CanUseConstruction();
        }
 
        return false;
