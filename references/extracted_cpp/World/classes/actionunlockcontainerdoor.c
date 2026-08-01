// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionunlockcontainerdoor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnlockShippingContainer: ActionUnlockDoors
{
    //custom condition, wrong key unlock attempt is allowed
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
        ContainerLockedBase shipCont;
        if (Class.CastTo(shipCont, target.GetObject()))
        {
            int doorIndex = TranslateLockSelectionIntoDoorIdx(target);
            if (doorIndex != -1)
                return shipCont.IsDoorLocked(doorIndex);
        }       
        return false;
