// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actionforcebitecharcoaltablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceBiteCharcoalTablets: ActionForceConsumeSingle
{
    override void ApplyModifiers( ActionData action_data )
    {
        PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
        ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
