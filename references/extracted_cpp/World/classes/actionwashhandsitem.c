// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionwashhandsitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWashHandsItem: ActionSingleUseBase
{
    protected const float WASH_HANDS_AMOUNT = 250; //ml
    
    void ActionWashHandsItem()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_CLEANHANDSBOTTLE;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_CLEANHANDSBOTTLE;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#wash_hands&quot;;
