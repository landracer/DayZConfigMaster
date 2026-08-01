// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionclapbeartrapwiththisitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionClapBearTrapWithThisItem: ActionSingleUseBase
{
    void ActionClapBearTrapWithThisItem()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONFB_POKE;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_FullBody      = true;
        m_Text          = &quot;#trigger_bear_trap&quot;;
