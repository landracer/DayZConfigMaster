// ======================================================================
// File: botstates_8c_source.html
// Category: other
// ======================================================================

class BotStateBase
{
PlayerBase m_Owner;
Bot m_Bot;
BotStateBase m_ParentState;
ref BotFSM m_FSM;
void BotStateBase (Bot bot = NULL, BotStateBase parent = NULL) { m_Bot = bot; m_Owner = bot.m_Owner; m_ParentState = parent; }
PlayerBase GetPlayerOwner() { return m_Owner; }
void SetParentState (BotStateBase parent) { m_ParentState = parent; }
BotStateBase GetParentState () { return m_ParentState; }
bool HasFSM () { return m_FSM != NULL; }
BotFSM GetFSM () { return m_FSM; }
bool ProcessEvent (BotEventBase e)
{
if (HasFSM())
return m_FSM.ProcessEvent(e);
return false;
}
void AddTransition (FSMTransition<BotStateBase, BotEventBase, BotActionBase, BotGuardBase> t)
{
if (HasFSM())
m_FSM.AddTransition(t);
else
Error("[botfsm] adding transition to state without FSM. Configure FSM first.");
}
void OnEntry (BotEventBase e)
{
if (HasFSM() && !m_FSM.IsRunning())
{
botDebugPrint("[botfsm] { " + this.Type().ToString() + " Has Sub-FSM! Starting submachine...");
m_FSM.Start(e);
}
else
botDebugPrint("[botfsm] { " + this.Type().ToString());
}
void OnUpdate (float dt)
{
if (HasFSM() && m_FSM.IsRunning())
m_FSM.GetCurrentState().OnUpdate(dt);
}
void OnAbort (BotEventBase e)
{
if (HasFSM() && m_FSM.IsRunning())
{
botDebugPrint("[botfsm] OnAbort " + this.Type().ToString() + " Has Sub-FSM! Aborting submachine...");
m_FSM.Abort(e);
}
botDebugPrint("[botfsm] } ABORTED " + this.Type().ToString());
}
void OnExit (BotEventBase e)
{
botDebugPrint("[botfsm] } " + this.Type().ToString());
}
bool IsWaitingForActionFinish () { return HasFSM() && m_FSM.IsRunning() && m_FSM.GetCurrentState().IsWaitingForActionFinish(); }
bool IsIdle () { return false; }
void OnSubMachineChanged (BotStateBase src, BotStateBase dst) { }
void OnStateChanged (BotStateBase src, BotStateBase dst) { }
};
class BotStateIdle : BotStateBase
{
void BotStateIdle (Bot bot = NULL, BotStateBase parent = NULL) { m_Bot = bot; m_Owner = m_Bot.m_Owner; m_ParentState = parent; }
override bool IsIdle () { return true; }
};