// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiongagself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionGagSelf: ActionContinuousBase
{   
    void ActionGagSelf()
    {
        m_CallbackClass = ActionCoverHeadSelfCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_COVERHEAD_SELF;
        //m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
        m_Text = &quot;#gag_self&quot;;
