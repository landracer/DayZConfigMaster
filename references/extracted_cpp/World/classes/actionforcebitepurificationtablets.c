// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actionforcebitepurificationtablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceBitePurificationTablets: ActionForceConsumeSingle
{
    override void ApplyModifiers( ActionData action_data )
    {
        PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
        //target.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
