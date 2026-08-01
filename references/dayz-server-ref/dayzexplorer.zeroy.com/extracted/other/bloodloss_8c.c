// ======================================================================
// File: bloodloss_8c_source.html
// Category: other
// ======================================================================

class BloodLoss extends SymptomBase
{
Material m_MatGauss;
const int BLUR_DURATION = 3000;
float m_BloodSet;
PPERequester_BloodLoss m_RequesterBloodLoss;
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.SECONDARY;
m_Priority = 0;
m_ID = SymptomIDs.SYMPTOM_BLOODLOSS;
m_DestroyOnAnimFinish = true;
m_IsPersistent = false;
m_SyncToClient = true;
m_BloodSet = -1;
if ( !g_Game.IsDedicatedServer() )
{
Class.CastTo(m_RequesterBloodLoss,PPERequester_BloodLoss.Cast(PPERequesterBank.GetRequester(PPERequester_BloodLoss)));
}
}
override bool AllowInUnconscious()
{
return true;
}
override void OnUpdateServer(PlayerBase player, float deltatime)
{
}
override void OnUpdateClient(PlayerBase player, float deltatime)
{
if ( player.IsPlayerSelected() && player.GetTransferValues() && player.GetTransferValues().GetBlood() != m_BloodSet )
{
m_BloodSet = player.GetTransferValues().GetBlood();
if (m_BloodSet < 1.0)
{
m_RequesterBloodLoss.SetBloodLossLevel(m_BloodSet);
}
else
{
m_RequesterBloodLoss.Stop();
}
}
}
override void OnGetActivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
}
override void OnGetActivatedClient(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
}
override void OnGetDeactivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetDeactivated", m_Player.ToString());
}
override void OnGetDeactivatedClient(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetDeactivated", m_Player.ToString());
}
}