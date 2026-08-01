// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handstatebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


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
