// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionturnoffpowergenerator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffPowerGenerator: ActionInteractBase
{   
    void ActionTurnOffPowerGenerator()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = &quot;#switch_off&quot;;
