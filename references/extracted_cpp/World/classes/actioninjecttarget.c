// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioninjecttarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInjectTarget: ActionSingleUseBase
{
    void ActionInjectTarget()
    {
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
        
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INJECTIONTARGET;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#inject_target&quot;;
