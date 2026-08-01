// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioneattabletfromwrapper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatTabletFromWrapper : ActionConsumeSingle
{
    void ActionEatTabletFromWrapper()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_EAT_TABLET;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_EAT_TABLET;
 
        m_Text = &quot;#eat&quot;;
