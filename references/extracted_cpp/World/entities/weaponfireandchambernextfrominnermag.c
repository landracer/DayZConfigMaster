// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponfireandchambernextfrominnermag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WeaponFireAndChamberNextFromInnerMag extends WeaponStateBase
{
    WeaponActions m_action;
    int m_actionType;
 
    float m_dtAccumulator;
    ref WeaponFire m_fire;
 
    void WeaponFireAndChamberNextFromInnerMag (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_action = action;
        m_actionType = actionType;
 
        // setup nested state machine
        m_fire = new WeaponFireAndChamber(m_weapon, this, m_action, m_actionType);
 
        // events
        WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
        WeaponEventAnimBulletEject  __be_ = new WeaponEventAnimBulletEject;
        WeaponEventReloadTimeout __to_ = new WeaponEventReloadTimeout;
 
        m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
 
        // transitions
        m_fsm.AddTransition(new WeaponTransition(m_fire, _fin_, NULL));
        m_fsm.AddTransition(new WeaponTransition(m_fire, __to_, NULL));
 
        m_fsm.SetInitialState(m_fire);
