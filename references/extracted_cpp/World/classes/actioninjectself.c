// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioninjectself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInjectSelf: ActionSingleUseBase
{
    void ActionInjectSelf()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INJECTION;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
        m_Text = &quot;#inject&quot;;
