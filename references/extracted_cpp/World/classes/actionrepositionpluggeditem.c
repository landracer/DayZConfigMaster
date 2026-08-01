// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionrepositionpluggeditem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepositionPluggedItem : ActionInteractBase
{
    // Through this action players can reposition already placed electric devices without unplugging them from the power source.
    void ActionRepositionPluggedItem()
    {
        m_CallbackClass = ActionInteractBaseCB;
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
 
        m_Text = &quot;#reposition&quot;;
