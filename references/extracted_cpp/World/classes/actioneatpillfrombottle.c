// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioneatpillfrombottle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatPillFromBottle : ActionConsumeSingle
{
    void ActionEatPillFromBottle()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_EAT_PILL;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_EAT_PILL;
 
        m_Text = &quot;#eat&quot;;
