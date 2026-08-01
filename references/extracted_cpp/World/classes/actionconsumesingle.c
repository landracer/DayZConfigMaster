// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionconsumesingle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionConsumeSingle : ActionSingleUseBase
{
    const int DEFAULT_CONSUMED_QUANTITY = 1;
    
    void ActionConsumeSingle()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY;
 
        m_Text = &quot;#eat&quot;;
