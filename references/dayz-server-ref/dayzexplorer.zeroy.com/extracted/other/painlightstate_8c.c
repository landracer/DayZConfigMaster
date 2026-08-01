// ======================================================================
// File: painlightstate_8c_source.html
// Category: other
// ======================================================================

class PainLightSymptom extends SymptomBase
{
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.PRIMARY;
m_Priority = 1;
m_ID = SymptomIDs.SYMPTOM_PAIN_LIGHT;
m_SyncToClient = true;
m_MaxCount = 2;
m_Duration = 1;
}
override void OnUpdateServer(PlayerBase player, float deltatime)
{
}
override void OnUpdateClient(PlayerBase player, float deltatime)
{
}
override void OnAnimationPlayFailed()
{
}
override bool CanActivate()
{
return true;
}
override void OnGetActivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable())
Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
PlaySound(EPlayerSoundEventID.TAKING_DMG_LIGHT);
}
override void OnGetActivatedClient(PlayerBase player)
{
//player.SpawnShockEffect(0.5);
CachedObjectsParams.PARAM2_FLOAT_FLOAT.param1 = 26;
CachedObjectsParams.PARAM2_FLOAT_FLOAT.param2 = 0.25;
player.SpawnDamageDealtEffect2(CachedObjectsParams.PARAM2_FLOAT_FLOAT);
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