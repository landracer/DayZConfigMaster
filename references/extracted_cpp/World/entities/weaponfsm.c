// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/weaponfsm.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class WeaponFSM extends HFSMBase<WeaponStateBase, WeaponEventBase, WeaponActionBase, WeaponGuardBase>
{
    private static const int MAX_SYNCHRONIZE_ATTEMPTS = 12;
    private static const int MIN_SYNCHRONIZE_INTERVAL = 3000; // ms
    private static const int RESET_SYNCHRONIZE_THRESHOLD = 3600000; // ms
    private int m_SynchronizeAttempts;
    private int m_LastSynchronizeTime;
    
    protected int m_NextStateId = 0; 
    protected ref array<WeaponStateBase> m_UniqueStates = new array<WeaponStateBase>; 
 
    protected void SetInternalID(WeaponStateBase state)
    {
        if (state && state.GetInternalStateID() == -1)
        {
            state.SetInternalStateID(m_NextStateId);
 
            //if (LogManager.IsWeaponLogEnable()) { wpnDebugSpam(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot; unique state=&quot; + state + &quot; has id=&quot; + m_NextStateId); }
            m_UniqueStates.Insert(state);
            ++m_NextStateId;
        }
