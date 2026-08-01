// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionungagself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUngagSelf: ActionContinuousBase
{
    void ActionUngagSelf()
    {
        m_CallbackClass = ActionUncoverHeadSelfCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_COVERHEAD_SELF;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#ungag&quot;;
