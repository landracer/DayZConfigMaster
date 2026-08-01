// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionlowerflag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ActionLowerFlag: ActionContinuousBase
{
    void ActionLowerFlag()
    {
        m_CallbackClass = ActionManipulateFlagCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_LOWER_FLAG;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#lower_flag&quot;;
