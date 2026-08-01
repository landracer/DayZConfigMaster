// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionfeedpurificationtablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFeedPurificationTablets: ActionForceConsume
{
    override void ApplyModifiers( ActionData action_data )
    {
        PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
        //ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
