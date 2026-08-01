// ======================================================================
// File: blindnessstate_8c_source.html
// Category: other
// ======================================================================

class BlindnessSymptom extends SymptomBase
{
Material m_MatGauss;
const int BLUR_DURATION = 3000;
ref Param1<vector> m_Position = new Param1<vector>("0 0 0");
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.SECONDARY;
m_Priority = 0;
m_ID = SymptomIDs.SYMPTOM_BLINDNESS;
m_DestroyOnAnimFinish = true;
m_IsPersistent = false;
MakeParamObjectPersistent(m_Position);
}
override void OnUpdateServer(PlayerBase player, float deltatime)
{
}
override void OnUpdateClient(PlayerBase player, float deltatime)
{
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
PPERequesterBank.GetRequester(PPERequester_BurlapSackEffects).Stop(); //TODO - use different PPERequester when connected, otherwise it would interfere with burlapsack blindness
}
}