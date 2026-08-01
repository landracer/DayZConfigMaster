// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionbuildshelter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBuildShelter: ActionContinuousBase
{
    string                      m_SoundsetBuildStart;
    string                      m_SoundsetBuildLoop;
    string                      m_SoundsetBuildFinish;
    ref protected EffectSound   m_BuildLoopSound;
    
    void ActionBuildShelter()
    {
        m_CallbackClass = ActionBuildPartCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#build_shelter_leather&quot;;
