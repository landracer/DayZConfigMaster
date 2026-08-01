// ======================================================================
// File: fsmbase_8c_source.html
// Category: other
// ======================================================================

void fsmbDebugPrint (string s)
{
#ifdef FSM_DEBUG
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#else
//Print("" + s); // comment/uncomment to hide/see debug logs
#endif
}
void fsmbDebugSpam (string s)
{
#ifdef FSM_DEBUG_SPAM
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#else
//Print("" + s); // comment/uncomment to hide/see debug logs
#endif
}
class FSMTransition<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
ref FSMStateBase m_srcState;
ref FSMEventBase m_event; // @NOTE: NULL event means "completion transition" in UML speak
ref FSMStateBase m_dstState; // @NOTE: NULL dst state == UML terminate pseudonode
ref FSMActionBase m_action;
ref FSMGuardBase m_guard;
void FSMTransition (FSMStateBase src, FSMEventBase e, FSMStateBase dst, FSMActionBase a = NULL, FSMGuardBase g = NULL)
{
m_srcState = src;
m_event = e;
m_dstState = dst;
m_action = a;
m_guard = g;
}
};
enum ProcessEventResult
{
FSM_OK,
FSM_TERMINATED,
FSM_ABORTED,
FSM_NO_TRANSITION,
};
class FSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
protected ref FSMStateBase m_state;
protected ref FSMStateBase m_initialState;
protected ref FSMEventBase m_initialEvent;
protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_transitions;
void FSMBase ()
{
m_transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
}
FSMStateBase GetCurrentState ()
{
return m_state;
}
void SetInitialState (FSMStateBase initial_state)
{
m_initialState = initial_state;
}
void Start (FSMEventBase initial_event = NULL)
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[fsm] " + this.ToString() + "::Start(" + initial_event.ToString() + "), init_state=" + m_initialState.ToString());
m_state = m_initialState;
m_state.OnEntry(initial_event);
}
bool IsRunning () { return m_state != NULL; }
void Terminate (FSMEventBase terminal_event = NULL)
{
if (IsRunning())
{
m_state.OnExit(terminal_event);
m_state = NULL;
}
}
void Update (float dt)
{
if (IsRunning())
m_state.OnUpdate(dt);
}
void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
{
m_transitions.Insert(t);
}
ProcessEventResult ProcessEvent (FSMEventBase e)
{
FSMStateBase curr_state = m_state;
int count = m_transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_transitions.Get(i);
if (row.m_srcState.Type() == curr_state.Type() && row.m_event.Type() == e.Type())
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_transitions.Get(i);
bool hasGuard = t.m_guard != NULL;
if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e))) // 1) exec guard (if any)
{
ProcessLocalTransition(t, e); // 2) process transition allowed by guard
}
}
}
return ProcessEventResult.FSM_NO_TRANSITION;
}
protected ProcessEventResult ProcessLocalTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[fsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());
m_state.OnExit(e); // 1) call onExit on old state
if (t.m_action)
t.m_action.Action(e); // 2) execute transition action (if any)
m_state = t.m_dstState; // 3) change state to new
if (t.m_dstState != NULL)
{
m_state.OnEntry(e); // 4a) call onEntry on new state
return ProcessEventResult.FSM_OK;
}
else
{
if (LogManager.IsInventoryHFSMLogEnable()) fsmbDebugPrint("[fsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
}
}
};