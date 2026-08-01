// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/vehicles/actionswitchseats.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ActionSwitchSeats: ActionBase
{
    void ActionSwitchSeats()
    {
        //m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_SpecialtyWeight = 1.0;
        m_LockTargetOnUse = false;
        m_Text = &quot;#change_seat&quot;;
