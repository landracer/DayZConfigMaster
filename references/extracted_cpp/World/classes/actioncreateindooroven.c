// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actioncreateindooroven.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCreateIndoorOven: ActionSingleUseBase
{
    void ActionCreateIndoorOven()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
        m_Text = &quot;#place_object&quot;;
