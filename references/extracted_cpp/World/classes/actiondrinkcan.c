// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondrinkcan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDrinkCan: ActionDrink
{
    void ActionDrinkCan()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_DRINK;
        m_Text = &quot;#drink&quot;;
