// ======================================================================
// File: dayzinfectedinputcontroller_8c_source.html
// Category: other
// ======================================================================

class DayZInfectedInputController : DayZCreatureAIInputController
{
proto native bool IsVault();
proto native float GetVaultHeight();
proto native int GetMindState();
proto native EntityAI GetTargetEntity();
//--------------------------------------------------------------
private void DayZInfectedInputController()
{
}
private void ~DayZInfectedInputController()
{
}
};