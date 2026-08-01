// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionpackshelter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDeconstructShelter : ActionContinuousBase
{
    void ActionDeconstructShelter()
    {
        m_CallbackClass     = ActionPackTentCB;
        m_SpecialtyWeight   = UASoftSkillsWeight.PRECISE_LOW;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text              = &quot;#pack_tent&quot;;
