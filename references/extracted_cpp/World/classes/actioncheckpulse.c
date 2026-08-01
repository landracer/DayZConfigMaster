// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actioncheckpulse.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionCheckPulse: ActionInteractBase
{
    const int TARGET_IRREGULAR_PULSE_BIT = 1 << 31;
    void ActionCheckPulse()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#check_pulse_target&quot;;
