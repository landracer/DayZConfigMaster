// ======================================================================
// File: botfsm_8c_source.html
// Category: other
// ======================================================================

class BotFSM extends HFSMBase<BotStateBase, BotEventBase, BotActionBase, BotGuardBase>
{ };
typedef FSMTransition<BotStateBase, BotEventBase, BotActionBase, BotGuardBase> BotTransition;