// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioninjectmorphineself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInjectMorphineSelf: ActionInjectSelf
{
    override void ApplyModifiers( ActionData action_data )
    {
        action_data.m_MainItem.OnApply(action_data.m_Player);
        //player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
