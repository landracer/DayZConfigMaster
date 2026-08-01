// ======================================================================
// File: actionforcedrinkalcohol_8c_source.html
// Category: other
// ======================================================================

class ActionForceDrinkAlcohol: ActionForceDrink
{
override void ApplyModifiers( ActionData action_data )
{
PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
//ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
}
};