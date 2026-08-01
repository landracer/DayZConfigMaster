// ======================================================================
// File: weaponstatebase_8c_source.html
// Category: other
// ======================================================================

class WeaponStateBase
{
Weapon_Base m_weapon;
WeaponStateBase m_parentState;
ref WeaponFSM m_fsm;
int m_InternalID = -1;
void WeaponStateBase (Weapon_Base w = NULL, WeaponStateBase parent = NULL) { m_weapon = w; m_parentState = parent; }
void SetParentState (WeaponStateBase parent) { m_parentState = parent; }
WeaponStateBase GetParentState () { return m_parentState; }
bool HasFSM () { return m_fsm != NULL; }
WeaponFSM GetFSM () { return m_fsm; }
void SetInternalStateID (int i) { m_InternalID = i; }
int GetInternalStateID () { return m_InternalID; }
bool SaveCurrentFSMState (ParamsWriteContext ctx)
{
if (HasFSM())
{
if (IsIdle())
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::SaveCurrentFSMState - idle state, skipping other substates"); }
return m_fsm.SaveCurrentFSMState(ctx);
}
else
{
// if parent state is !idle (unstable) then save whole machine
if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::SaveCurrentFSMState - NOT idle state, saving full submachine state"); }
return m_fsm.SaveCurrentUnstableFSMState(ctx);
}
return false;
}
return true;
}
bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
{
if (HasFSM())
{
if (IsIdle())
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::LoadCurrentFSMState - idle state, skipping other substates"); }
if (m_fsm.LoadCurrentFSMState(ctx, version))
return true;
else
Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::LoadCurrentFSMState - Cannot load stable state for weapon=" + this);
}
else
{
// if parent state is !idle (unstable) then load whole machine
if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::LoadCurrentFSMState - NOT idle state, loading full submachine state"); }
if (m_fsm.LoadCurrentUnstableFSMState(ctx, version))
return true;
else
Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponStateBase::LoadCurrentFSMState - Cannot load unstable state for weapon=" + this);
}
return false;
}
return true;
}
bool ProcessEvent (WeaponEventBase e)
{
if (HasFSM())
return m_fsm.ProcessEvent(e);
return false;
}
void AddTransition (WeaponTransition t)
{
if (HasFSM())
m_fsm.AddTransition(t);
else
Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " adding transition to state without FSM. Configure FSM first.");
}
void OnEntry (WeaponEventBase e)
{
if (HasFSM() && !m_fsm.IsRunning())
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString() + " Has Sub-FSM! Starting submachine..."); }
m_fsm.Start(e);
}
else
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " { " + this.Type().ToString()); }
}
void OnUpdate (float dt)
{
if (HasFSM() && m_fsm.IsRunning())
m_fsm.GetCurrentState().OnUpdate(dt);
}
void OnAbort (WeaponEventBase e)
{
if (HasFSM() && m_fsm.IsRunning())
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " OnAbort " + this.Type().ToString() + " Has Sub-FSM! Aborting submachine..."); }
m_fsm.Abort(e);
}
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } ABORTED " + this.Type().ToString()); }
}
void OnExit (WeaponEventBase e)
{
if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " } " + this.Type().ToString()); }
}
bool IsWaitingForActionFinish () { return HasFSM() && m_fsm.IsRunning() && m_fsm.GetCurrentState().IsWaitingForActionFinish(); }
bool IsIdle () { return false; }
bool IsBoltOpen () { return false; }
void OnSubMachineChanged (WeaponStateBase src, WeaponStateBase dst) { }
void OnStateChanged (WeaponStateBase src, WeaponStateBase dst) { }
float GetCoolDown() { return 0; }
};