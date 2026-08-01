// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiondefibrilatebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDefibrilateBase: ActionContinuousBase
{   
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {   
        Defibrillator defib;
        Class.CastTo(defib, item);
        
        return CanDefibrillate(defib);
