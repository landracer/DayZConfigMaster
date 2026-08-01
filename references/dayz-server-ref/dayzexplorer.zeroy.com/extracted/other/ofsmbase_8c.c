// ======================================================================
// File: ofsmbase_8c_source.html
// Category: other
// ======================================================================

class OFSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
protected ref array<ref FSMStateBase> m_States;
protected ref array<ref FSMStateBase> m_InitialStates;
protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_Transitions;
void OFSMBase ()
{
m_States = new array<ref FSMStateBase>;
m_InitialStates = new array<ref FSMStateBase>;
m_Transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
}
array<ref FSMStateBase> GetCurrentState ()
{
return m_States;
}
void SetInitialStates (array<ref FSMStateBase> initial_states)
{
m_InitialStates = initial_states;
for (int s = 0; s < initial_states.Count(); ++s)
m_States.Insert(initial_states[s]);
}
void Start (array<ref FSMEventBase> initial_events = null)
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[ofsm] " + this.ToString() + "::Start(" + initial_events.ToString() + "), init_state=" + m_InitialStates.ToString());
for (int s = 0; s < m_States.Count(); ++s)
{
m_States[s] = m_InitialStates[s];
if (initial_events)
m_States[s].OnEntry(initial_events[s]);
else
m_States[s].OnEntry(null);
}
}
bool IsRunning ()
{
int sc = m_States.Count();
if (sc)
{
for (int s = 0; s < sc; ++s)
if (m_States[s] != null)
return true;
}
return false;
}
void Terminate (array<ref FSMEventBase> terminal_events = null)
{
if (IsRunning())
{
for (int s = 0; s < m_States.Count(); ++s)
{
if (terminal_events)
m_States[s].OnExit(terminal_events[s]);
else
m_States[s].OnExit(null);
m_States[s] = null;
}
}
}
void Update (float dt)
{
if (IsRunning())
{
for (int s = 0; s < m_States.Count(); ++s)
{
m_States[s].OnUpdate(dt);
}
}
}
void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
{
m_Transitions.Insert(t);
}
ProcessEventResult ProcessEvent (FSMEventBase e)
{
int count = m_Transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(i);
if (row.m_event.Type() == e.Type())
{
for (int s = 0; s < m_States.Count(); ++s)
{
if (row.m_srcState.Type() == m_States[s].Type() && row.m_event.Type() == e.Type())
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
bool hasGuard = t.m_guard != NULL;
if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e))) // 1) exec guard (if any)
{
ProcessLocalTransition(s, t, e); // 2) process transition allowed by guard
}
}
}
}
}
return ProcessEventResult.FSM_NO_TRANSITION;
}
protected ProcessEventResult ProcessLocalTransition (int s, FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[ofsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());
m_States[s].OnExit(e); // 1) call onExit on old state
if (t.m_action)
t.m_action.Action(e); // 2) execute transition action (if any)
m_States[s] = t.m_dstState; // 3) change state to new
if (t.m_dstState != NULL)
{
m_States[s].OnEntry(e); // 4a) call onEntry on new state
return ProcessEventResult.FSM_OK;
}
else
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[ofsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
}
}
};