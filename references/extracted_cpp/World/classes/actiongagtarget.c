// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiongagtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionGagTarget: ActionContinuousBase
{   
    void ActionGagTarget()
    {
        m_CallbackClass = ActionCoverHeadTargetCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_COVERHEAD_TARGET;
        //m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
        m_Text = &quot;#gag_person&quot;;
