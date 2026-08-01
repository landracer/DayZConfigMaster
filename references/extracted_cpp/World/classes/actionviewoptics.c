// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionviewoptics.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionViewOptics : ActionContinuousBase
{
    void ActionViewOptics()
    {
        m_CallbackClass     = ActionRaiseAndViewCB;
        m_CommandUID        = DayZPlayerConstants.CMD_GESTUREFB_LOOKOPTICS;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_GESTUREFB_LOOKOPTICS;
 
        m_Text = &quot;#Look_Through&quot;;
