// ======================================================================
// File: actiondrinkdisinfectant_8c_source.html
// Category: other
// ======================================================================

class ActionDrinkDisinfectant: ActionDrink
{
override void ApplyModifiers( ActionData action_data )
{
action_data.m_Player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
}
};