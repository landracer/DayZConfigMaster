// ======================================================================
// File: handstatebase_8c_source.html
// Category: other
// ======================================================================

class HandStateBase
{
Man m_Player;
HandStateBase m_parentState;
ref HandFSM m_FSM;
void HandStateBase (Man player = NULL, HandStateBase parent = NULL) { m_Player = player; m_parentState = parent; }
void SetParentState (HandStateBase parent) { m_parentState = parent; }
HandStateBase GetParentState () { return m_parentState; }
bool HasFSM () { return m_FSM != NULL; }
HandFSM GetFSM () { return m_FSM; }
bool ProcessEvent (HandEventBase e)
{
if (HasFSM())
return m_FSM.ProcessEvent(e);
return false;
}
void AddTransition (HandTransition t)
{
if (HasFSM())
m_FSM.AddTransition(t);
else
Error("[hndfsm] adding transition to state without FSM. Configure FSM first.");
}
void OnEntry (HandEventBase e)
{
if (HasFSM() && !m_FSM.IsRunning())
{
if (e)
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] { " + Object.GetDebugName(e.m_Player) + " STS = " + e.m_Player.GetSimulationTimeStamp() + " " + this.Type().ToString() + " Has Sub-FSM! Starting submachine... OnEntry");
}
else
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] { " + this.Type().ToString() + " Has Sub-FSM! Starting submachine... OnEntry");
}
m_FSM.Start(e);
}
else
{
if (e)
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] { " + Object.GetDebugName(e.m_Player) + " STS = " + e.m_Player.GetSimulationTimeStamp() + " " + this.Type().ToString() + " OnEntry");
}
else
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] { " + this.Type().ToString() + " OnEntry");
}
}
}
void OnUpdate (float dt)
{
if (HasFSM() && m_FSM.IsRunning())
m_FSM.GetCurrentState().OnUpdate(dt);
}
void OnAbort (HandEventBase e)
{
if (HasFSM() && m_FSM.IsRunning())
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] OnAbort " + this.Type().ToString() + " Has Sub-FSM! Aborting submachine... OnAbort");
m_FSM.Abort(e);
}
//Debug.InventoryHFSMLog("ABORTED " + e.m_Player.GetSimulationTimeStamp(), ""/*typename.EnumToString(HandEventID, GetEventID()) */, "n/a", "OnAbort", m_Player.ToString() );
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] } " + Object.GetDebugName(e.m_Player) + " STS = " + e.m_Player.GetSimulationTimeStamp() + " ABORTED " + this.Type().ToString() + " OnAbort");
}
void OnExit (HandEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable()) hndDebugPrint("[hndfsm] } " + Object.GetDebugName(e.m_Player) + " STS = " + e.m_Player.GetSimulationTimeStamp() + " " + this.Type().ToString() + " OnExit");
}
bool IsWaitingForActionFinish () { return HasFSM() && m_FSM.IsRunning() && m_FSM.GetCurrentState().IsWaitingForActionFinish(); }
bool IsIdle () { return false; }
void OnSubMachineChanged (HandStateBase src, HandStateBase dst) { }
void OnStateChanged (HandStateBase src, HandStateBase dst)
{
m_Player.GetHumanInventory().OnHandsStateChanged(src, dst);
}
};