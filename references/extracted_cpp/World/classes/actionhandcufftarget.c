// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionhandcufftarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionHandcuffTarget: ActionSingleUseBase
{
    void ActionHandcuffTarget()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HANDCUFFTARGET;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
        m_Text = &quot;#restrain&quot;;
