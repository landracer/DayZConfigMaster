// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionfillbottlesnow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillBottleSnow: ActionFillBottleBase
{
    void ActionFillBottleSnow()
    {
        m_CallbackClass     = ActionFillBottleBaseCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONFB_FILLING_CONTAINER_SNOW;
        m_FullBody          = true;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_SpecialtyWeight   = UASoftSkillsWeight.PRECISE_LOW;
        m_Text              = &quot;#fill&quot;;
        
        m_AllowedLiquidMask = LIQUID_SNOW;
