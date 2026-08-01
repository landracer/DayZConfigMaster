// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnonchemlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionTurnOnChemlight: ActionTurnOnWhileInHands
{
    void ActionTurnOnChemlight()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
        m_Text = &quot;#activate&quot;;
