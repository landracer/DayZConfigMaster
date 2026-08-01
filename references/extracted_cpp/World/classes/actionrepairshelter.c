// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepairshelter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//used for shelters (non-proxy tent objects)
class ActionRepairShelter: ActionRepairTent
{
    void ActionRepairShelter()
    {
        m_CallbackClass = ActionRepairTentCB;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
        
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
