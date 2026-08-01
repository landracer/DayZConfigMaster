// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiondrinkdisinfectant.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionDrinkDisinfectant: ActionDrink
{
    override void ApplyModifiers( ActionData action_data )
    {
        action_data.m_Player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
