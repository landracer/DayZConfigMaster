// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actioneatcharcoaltablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatCharcoalTablets: ActionConsume
{
    void ActionEatCharcoalTablets()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
