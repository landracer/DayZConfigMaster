// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionworldliquidactionswitch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//can be eventually extended to allow switching for multiple action types?
class ActionWorldLiquidActionSwitch: ActionSingleUseBase
{
    bool    m_switch_to;
    void ActionWorldLiquidActionSwitch()
    {
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
