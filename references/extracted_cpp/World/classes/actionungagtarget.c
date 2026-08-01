// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionungagtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUngagTarget: ActionContinuousBase
{
    void ActionUngagTarget()
    {
        m_CallbackClass = ActionUncoverHeadTargetCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_COVERHEAD_TARGET;
        //m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#ungag_person&quot;;
