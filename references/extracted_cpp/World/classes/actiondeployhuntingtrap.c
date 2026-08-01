// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/deployactions/actiondeployhuntingtrap.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDeployHuntingTrap : ActionDeployBase
{
    void ActionDeployHuntingTrap()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Sound             = &quot;craft_universal_0&quot;;
