// ======================================================================
// File: actionturnonchemlight_8c_source.html
// Category: other
// ======================================================================

class ActionTurnOnChemlight: ActionTurnOnWhileInHands
{
void ActionTurnOnChemlight()
{
m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
m_Text = "#activate";
}
};