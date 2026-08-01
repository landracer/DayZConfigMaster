// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionforceabitecan.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionForceABiteCan: ActionForceABite
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {   
        return true;
        /* 
        //if staging for cans is introduced
        if ( item && item.IsOpen() )
        {
            return true;
        }
        return false;
        */
