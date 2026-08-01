// ======================================================================
// File: hfsmbase_8c_source.html
// Category: other
// ======================================================================

void fsmDebugPrint (string s)
{
#ifdef FSM_DEBUG
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#else
//Print("" + s); // comment/uncomment to hide/see debug logs
#endif
}
void fsmDebugSpam (string s)
{
#ifdef FSM_DEBUG_SPAM
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#else
//Print("" + s); // comment/uncomment to hide/see debug logs
#endif
}
class HFSMBase<Class FSMStateBase, Class FSMEventBase, Class FSMActionBase, Class FSMGuardBase>
{
protected ref FSMStateBase m_State;
protected FSMStateBase m_OwnerState;
protected ref FSMStateBase m_InitialState;
protected ref array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>> m_Transitions = new array<ref FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase>>;
protected bool m_HasCompletions = false;
void HFSMBase (FSMStateBase ownerState = NULL)
{
m_OwnerState = ownerState;
}
FSMStateBase GetCurrentState ()
{
return m_State;
}
void SetCurrentState (FSMStateBase state)
{
if(m_State != state)
{
m_State = state;
}
}
FSMStateBase GetOwnerState ()
{
return m_OwnerState;
}
bool GetHierarchyPath (FSMStateBase state, out array<FSMStateBase> path)
{
FSMStateBase curr = state;
while (curr)
{
path.Insert(curr);
curr = curr.GetParentState();
}
return path.Count() > 0;
}
void SetInitialState (FSMStateBase initial_state)
{
m_InitialState = initial_state;
}
void AddTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t)
{
m_Transitions.Insert(t);
//if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[hfsm] +++ ow=" + this.GetOwnerState() + " this=" + this + " t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
if (t.m_event == NULL)
{
Print("Warning (performance): FSM " + this + " has completion transition for src=" + t.m_srcState + " ---NULL----|> dst=" + t.m_dstState);
m_HasCompletions = true;
}
}
void Start (FSMEventBase initial_event = NULL, bool useExistingState = false)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] " + this.ToString() + "::Start(" + initial_event.ToString() + "), init_state=" + m_InitialState.ToString());
}
if (!useExistingState)
m_State = m_InitialState;
m_State.OnEntry(initial_event);
if (m_HasCompletions)
ProcessCompletionTransitions();
}
bool IsRunning () { return m_State != NULL; }
void Terminate (FSMEventBase terminal_event = NULL)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] " + this.ToString() + "::Terminate(" + terminal_event.ToString() + ")");
}
if (IsRunning())
{
m_State.OnExit(terminal_event);
m_State = NULL;
}
}
void Abort (FSMEventBase abort_event = NULL)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] " + this.ToString() + "::Abort(" + abort_event.ToString() + ")");
}
if (IsRunning())
{
m_State.OnAbort(abort_event);
m_State = NULL;
}
}
void Update (float dt)
{
if (IsRunning())
m_State.OnUpdate(dt);
}
protected ProcessEventResult ProcessAbortTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] (local abort) state=" + t.m_srcState.ToString() + "-------- ABORT event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());
}
m_State.OnAbort(e); // 1) call onAbort on old state
if (t.m_action)
t.m_action.Action(e); // 2) execute transition action (if any)
auto tmp = t.m_srcState.GetParentState();
if (tmp == t.m_dstState.GetParentState())
{
m_State = t.m_dstState; // 3) change state to new (or NULL)
if (t.m_dstState != NULL)
{
m_State.OnEntry(e); // 4a1) call onEntry on new state (see 4a2) )
return ProcessEventResult.FSM_OK;
}
else
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] abort & terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
}
return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
}
}
else
{
m_State = NULL;
return ProcessEventResult.FSM_ABORTED; // 4c) or signal abort to parent (with appropriate transition)
}
}
FSMStateBase FindAbortDestinationState (FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
if (GetOwnerState())
fsmDebugPrint("[hfsm] SUB! " + GetOwnerState().Type().ToString() + "::FindAbortDestinationState(" + e.Type().ToString() + ")");
else
fsmDebugPrint("[hfsm] root::FindAbortDestinationState(" + e.Type().ToString() + ")");
}
// 1) look in submachine first (if any)
if (m_State && m_State.HasFSM())
{
HFSMBase<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> a = m_State.GetFSM();
FSMStateBase abort_dst = a.FindAbortDestinationState(e);
if (abort_dst)
{
return abort_dst;
}
}
// 2) local transitions
int i = FindFirstUnguardedTransition(e);
if (i == -1)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] abort event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
}
return NULL;
}
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
return t.m_dstState;
}
FSMStateBase ProcessAbortEvent(FSMEventBase e, out ProcessEventResult result)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
if (GetOwnerState())
fsmDebugPrint("[hfsm] SUB! " + GetOwnerState().Type().ToString() + "::ProcessAbortEvent(" + e.Type().ToString() + ")");
else
fsmDebugPrint("[hfsm] root::ProcessAbortEvent(" + e.Type().ToString() + ")");
}
// 1) look in submachine first (if any)
if (m_State && m_State.HasFSM())
{
HFSMBase<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> a = m_State.GetFSM();
ProcessEventResult subfsm_res;
FSMStateBase abort_dst = a.ProcessAbortEvent(e, subfsm_res);
switch (subfsm_res)
{
case ProcessEventResult.FSM_OK:
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] event processed by sub machine=" + m_State.ToString());
}
result = subfsm_res; // 1.1) submachine accepted event
return NULL;
}
case ProcessEventResult.FSM_ABORTED:
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString());
}
m_State.OnAbort(e); // 1.2) submachine aborted, abort submachine owner (i.e. this)
if (GetOwnerState() == abort_dst.GetParentState())
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString() + " & abort destination reached.");
}
m_State = abort_dst;
m_State.OnEntry(e); // 1.3) submachine aborted, call onEntry on new state (cross-hierarchy transition)
result = ProcessEventResult.FSM_OK;
return NULL;
}
else
{
result = ProcessEventResult.FSM_ABORTED; // 1.4) submachine has aborted, look for destination state in parent
return NULL;
}
break;
}
case ProcessEventResult.FSM_TERMINATED:
{
break; // submachine has finished, look for local transitions from exited submachine
}
case ProcessEventResult.FSM_NO_TRANSITION:
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] aborted (but no transition) sub machine=" + m_State.ToString());
}
break; // submachine has no transition, look for transitions in local machine
}
}
}
// 2) local transitions
int i = FindFirstUnguardedTransition(e);
if (i == -1)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] abort event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
}
result = ProcessEventResult.FSM_NO_TRANSITION;
return NULL;
}
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
ProcessEventResult res = ProcessAbortTransition(t, e);
result = res;
switch (res)
{
case ProcessEventResult.FSM_OK:
{
//if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[hfsm] abort event processed by machine=" + m_State.ToString());
return NULL; // machine accepted event
}
case ProcessEventResult.FSM_ABORTED:
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] aborted sub machine=" + m_State.ToString() + " will fall-through to dst=" + t.m_dstState);
}
return t.m_dstState; // store destination state for parent(s)
}
case ProcessEventResult.FSM_TERMINATED:
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] aborted & terminated sub machine=" + m_State.ToString());
}
break; // submachine has finished, look for local transitions from exited submachine
}
case ProcessEventResult.FSM_NO_TRANSITION:
{
break; // submachine has no transition, look for transitions in local machine
}
}
return NULL;
}
ProcessEventResult ProcessEvent(FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
if (GetOwnerState())
fsmDebugPrint("[hfsm] SUB!::" + GetOwnerState().Type().ToString() + "::ProcessEvent(" + e.Type().ToString() + ")");
else
fsmDebugPrint("[hfsm] root::ProcessEvent(" + e.Type().ToString() + " =" + e.DumpToString());
}
// 1) completion transitions have priority (if any)
if (m_HasCompletions)
ProcessCompletionTransitions();
// 2) submachine then (if any)
if (m_State && m_State.HasFSM())
{
ProcessEventResult subfsm_res = m_State.ProcessEvent(e);
switch (subfsm_res)
{
case ProcessEventResult.FSM_OK:
{
if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[hfsm] event processed by sub machine=" + m_State.ToString());
return subfsm_res; // submachine accepted event
}
case ProcessEventResult.FSM_TERMINATED:
{
break; // submachine has finished, look for local transitions from exited submachine
}
case ProcessEventResult.FSM_NO_TRANSITION:
{
break; // submachine has no transition, look for transitions in local machine
}
}
}
// 3) local transitions
int i = FindFirstUnguardedTransition(e);
if (i == -1)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] event has no transition: src=" + m_State.ToString() + " e=" + e.Type().ToString());
}
return ProcessEventResult.FSM_NO_TRANSITION;
}
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(i);
ProcessEventResult res;
if (row.m_dstState != NULL)
{
// this is regular transition
if (row.m_srcState.GetParentState() == row.m_dstState.GetParentState())
res = LocalTransition(i, e); // transition is within this state machine
else
Error("cross-hierarchy transition or misconfigured transition detected!");
//res = HierarchicTransition(i, e); // transition has to cross hierarchy
}
else
{
// this is terminating transition
if (row.m_srcState.GetParentState() == GetOwnerState())
res = LocalTransition(i, e); // terminating transition is within this state machine
else
Error("cross-hierarchy transition or misconfigured transition detected!");
//res = HierarchicTransition(i, e); // source node crosses hierarchy (terminate lies always within this machine)
}
return res;
}
protected int FindFirstUnguardedTransition (FSMEventBase e)
{
FSMStateBase curr_state = m_State;
int count = m_Transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
if ((t.m_srcState == curr_state) && (t.m_event != NULL) && (t.m_event.Type() == e.Type()))
{
//if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[hfsm] [" + i + "/" + count + "] *** matched! t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
bool hasGuard = t.m_guard != NULL;
if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e))) // 1) exec guard (if any)
{
return i;
}
}
//else if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[hfsm][" + i + "/" + count + "] ... matching t=" + t + " state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
}
return -1;
}
FSMStateBase FindTransitionState(FSMStateBase s, FSMEventBase e)
{
FSMStateBase curr_state = s;
int count = m_Transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
if ((t.m_srcState == curr_state) && (t.m_event != NULL) && (t.m_event.Type() == e.Type()))
{
return t.m_dstState;
}
}
return null;
}
FSMStateBase FindGuardedTransitionState(FSMStateBase s, FSMEventBase e)
{
FSMStateBase curr_state = s;
int count = m_Transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
if ((t.m_srcState == curr_state) && (t.m_event != NULL) && (t.m_event.Type() == e.Type()))
{
bool hasGuard = t.m_guard != NULL;
if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(e))) // 1) exec guard (if any)
{
return t.m_dstState;
}
}
}
return null;
}
protected int FindFirstCompletionTransition ()
{
if (IsRunning())
{
FSMStateBase curr_state = m_State;
int count = m_Transitions.Count();
for (int i = 0; i < count; ++i)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
//if (LogManager.IsInventoryHFSMLogEnable()) fsmDebugPrint("[hfsm] (local) matching state=" + t.m_srcState + "-------- event=" + t.m_event + "[G=" + t.m_guard +"]/A=" + t.m_action + " --------|> dst=" + t.m_dstState);
if ((t.m_srcState.Type() == curr_state.Type()) && (t.m_event == NULL))
{
bool hasGuard = t.m_guard != NULL;
if (!hasGuard || (hasGuard && t.m_guard.GuardCondition(NULL))) // 1) exec guard (if any)
{
return i;
}
}
}
}
return -1;
}
protected ProcessEventResult ProcessLocalTransition (FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t, FSMEventBase e)
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] (local) state=" + t.m_srcState.ToString() + "-------- event=" + e.ToString() + "[G=" + t.m_guard.ToString() +"]/A=" + t.m_action.ToString() + " --------|> dst=" + t.m_dstState.ToString());
}
m_State.OnExit(e); // 1) call onExit on old state
if (t.m_action)
t.m_action.Action(e); // 2) execute transition action (if any)
m_State = t.m_dstState; // 3) change state to new
if (t.m_dstState != NULL)
{
m_State.OnEntry(e); // 4a) call onEntry on new state
if (GetOwnerState())
GetOwnerState().OnSubMachineChanged(t.m_srcState, t.m_dstState); // 5a) notify owner state about change in submachine
if (m_State)
m_State.OnStateChanged(t.m_srcState, t.m_dstState); // 5b) notify current state about change in machine
return ProcessEventResult.FSM_OK;
}
else
{
if (LogManager.IsInventoryHFSMLogEnable())
{
fsmDebugPrint("[hfsm] terminating fsm: state=" + t.m_srcState.ToString() + " event=" + e.ToString());
}
if (GetOwnerState())
GetOwnerState().OnSubMachineChanged(t.m_srcState, NULL); // 5) notify owner state about change in submachine
return ProcessEventResult.FSM_TERMINATED; // 4b) or terminate
}
}
protected ProcessEventResult LocalTransition (int i, FSMEventBase e)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> t = m_Transitions.Get(i);
ProcessEventResult ret = ProcessLocalTransition(t, e);
return ret;
}
protected ProcessEventResult ProcessCompletionTransitions ()
{
int completionIdx = FindFirstCompletionTransition();
while (completionIdx != -1)
{
FSMTransition<FSMStateBase, FSMEventBase, FSMActionBase, FSMGuardBase> row = m_Transitions.Get(completionIdx);
ProcessEventResult res;
if (row.m_dstState != NULL)
{
// this is regular completion transition
if (row.m_srcState.GetParentState() == row.m_dstState.GetParentState())
res = LocalTransition(completionIdx, NULL); // transition is within this state machine
else
Error("cross-hierarchy transition or misconfigured transition detected!");
//res = HierarchicTransition(completionIdx, NULL); // transition has to cross hierarchy
}
else
{
// this is terminating completion transition
if (row.m_srcState.GetParentState() == GetOwnerState())
res = LocalTransition(completionIdx, NULL); // terminating transition is within this state machine
else
Error("cross-hierarchy transition or misconfigured transition detected!");
//res = HierarchicTransition(completionIdx, NULL); // source node crosses hierarchy (terminate lies always within this machine)
}
completionIdx = FindFirstCompletionTransition();
}
return ProcessEventResult.FSM_NO_TRANSITION;
}
};