// ======================================================================
// File: freezestate_8c_source.html
// Category: other
// ======================================================================

class FreezeSymptom extends SymptomBase
{
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.PRIMARY;
m_Priority = 1;
m_ID = SymptomIDs.SYMPTOM_FREEZE;
m_DestroyOnAnimFinish = true;
m_SyncToClient = false;
m_MaxCount = 2;
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
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
HumanMovementState hms = new HumanMovementState();
player.GetMovementState(hms);
ItemBase item = m_Player.GetItemInHands();
if (!(item && item.IsHeavyBehaviour()) && !m_Player.IsSurrendered() && m_Manager.GetCurrentCommandID() == DayZPlayerConstants.COMMANDID_MOVE && hms.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_IDLE && !player.IsRestrained() && !player.IsInProne())
{
PlayAnimationADD(2);
}
else
{
PlaySound(EPlayerSoundEventID.FREEZING);
}
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
override SmptAnimMetaBase SpawnAnimMetaObject()
{
return new HeatComfortmMetaADD();
}
}
class FreezeRattleSymptom extends SymptomBase
{
//this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
override void OnInit()
{
m_SymptomType = SymptomTypes.PRIMARY;
m_Priority = 100;
m_ID = SymptomIDs.SYMPTOM_FREEZE_RATTLE;
m_DestroyOnAnimFinish = true;
m_SyncToClient = false;
m_MaxCount = 2;
m_Duration = 2;
}
override bool CanActivate()
{
return true;
}
override void OnGetActivatedServer(PlayerBase player)
{
if (LogManager.IsSymptomLogEnable()) Debug.SymptomLog("n/a", this.ToString(), "n/a", "OnGetActivated", m_Player.ToString());
PlaySound(EPlayerSoundEventID.RATTLING_TEETH);
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