// ======================================================================
// File: handstablestate_8c_source.html
// Category: other
// ======================================================================

class HandStableState extends HandStateBase
{
int m_AnimState;
void HandStableState (Man player = NULL, HandStateBase parent = NULL, int anim_state = -1) { m_AnimState = anim_state; }
void SyncAnimState () { }
override void OnEntry (HandEventBase e)
{
super.OnEntry(e);
SyncAnimState();
//m_weapon.OnStableStateEntry();
}
override void OnUpdate (float dt)
{
super.OnUpdate(dt);
SyncAnimState();
}
override void OnAbort (HandEventBase e)
{
super.OnAbort(e);
}
override void OnExit (HandEventBase e)
{
//m_weapon.ResetWeaponAnimState();
super.OnExit(e);
}
override bool IsIdle () { return true; }
int GetCurrentStateID () { return HandStateID.UNKNOWN; }
bool HasEntityInHands () { return false; }
};