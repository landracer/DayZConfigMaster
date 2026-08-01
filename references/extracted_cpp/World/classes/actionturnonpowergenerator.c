// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionturnonpowergenerator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOnPowerGenerator: ActionInteractBase
{   
    void ActionTurnOnPowerGenerator()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = &quot;#switch_on&quot;;
