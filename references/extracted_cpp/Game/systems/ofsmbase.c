// Source: F:/Games/Dayz/scripts/3_game/systems/ofsmbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


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
