// ======================================================================
// File: gaspsymptom_8c_source.html
// Category: other
// ======================================================================

class GaspSymptom extends SymptomBase
{
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.PRIMARY;
m_Priority = 100;
m_ID = SymptomIDs.SYMPTOM_GASP;
m_DestroyOnAnimFinish = true;
m_SyncToClient = false;
m_Duration = 2.5;
}
override bool CanBeInterupted()
{
return false;
}
override void OnGetActivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable())
Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
PlaySound(EPlayerSoundEventID.SYMPTOM_GASP);
}
override void OnGetActivatedClient(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable())
Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
}
override void OnGetDeactivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable())
Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetDeactivated", m_Player.ToString());
}
override void OnGetDeactivatedClient(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable())
Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetDeactivated", m_Player.ToString());
}
}