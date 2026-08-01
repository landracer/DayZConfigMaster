// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionmeasurebattery.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMeasureBattery: ActionSingleUseBase
{
 
    void ActionMeasureBattery()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY;
        m_Text = &quot;#lick_battery&quot;;
