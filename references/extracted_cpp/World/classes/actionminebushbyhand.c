// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionminebushbyhand.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMineBushByHand: ActionMineBush
{
    void ActionMineBushByHand()
    {
        m_CallbackClass = ActionMineBushCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
