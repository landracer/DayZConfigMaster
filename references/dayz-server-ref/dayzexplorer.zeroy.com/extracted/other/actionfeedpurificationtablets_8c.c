// ======================================================================
// File: actionfeedpurificationtablets_8c_source.html
// Category: other
// ======================================================================

class ActionFeedPurificationTablets: ActionForceConsume
{
override void ApplyModifiers( ActionData action_data )
{
PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
//ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
}
};