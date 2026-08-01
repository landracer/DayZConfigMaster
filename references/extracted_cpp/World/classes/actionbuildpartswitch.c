// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionbuildpartswitch.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionBuildPartSwitch: ActionSingleUseBase
{
    void ActionBuildPartSwitch()
    {
        //m_StanceMask = DayZPlayerConstants.STANCEMASK_NOTRAISED;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
        m_FullBody = true;
        m_Text = &quot;#switch_to_the_next_part&quot;;
