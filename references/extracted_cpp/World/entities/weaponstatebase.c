// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponstatebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


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
                if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot; WeaponStateBase::SaveCurrentFSMState - idle state, skipping other substates&quot;); }
                return m_fsm.SaveCurrentFSMState(ctx);
            }
            else
            {
                // if parent state is !idle (unstable) then save whole machine
                if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot; WeaponStateBase::SaveCurrentFSMState - NOT idle state, saving full submachine state&quot;); }
                return m_fsm.SaveCurrentUnstableFSMState(ctx);
            }
            return false;
        }
        return true;
