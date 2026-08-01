// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponcharging.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class WeaponCharging extends WeaponStateBase
{
    WeaponActions m_action;
    int m_actionType;
 
    ref WeaponCharging_Start m_start;
    ref WeaponCharging_CK m_onCK;
    ref WeaponChamberFromAttMag_W4T m_chamber;
    ref WeaponEjectAndChamberFromAttMag m_ejectChamber;
    ref WeaponEjectAllMuzzles m_eject;
 
    void WeaponCharging (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_action = action;
        m_actionType = actionType;
 
        // setup nested state machine
        m_start = new WeaponCharging_Start(m_weapon, this, m_action, m_actionType);
        m_onCK = new WeaponCharging_CK(m_weapon, this);
        m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
        m_ejectChamber = new WeaponEjectAndChamberFromAttMag(m_weapon, this);
        m_eject = new WeaponEjectAllMuzzles(m_weapon, this);
 
        // events
        WeaponEventBase __be_ = new WeaponEventAnimBulletEject;
        WeaponEventBase __bh_ = new WeaponEventAnimBulletHide;
        WeaponEventBase __ck_ = new WeaponEventAnimCocked;
        WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
 
        m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
        // transitions
        m_fsm.AddTransition(new WeaponTransition(  m_start, __be_, m_ejectChamber, NULL, new WeaponGuardHasAmmo(m_weapon) ));
        m_fsm.AddTransition(new WeaponTransition(  m_start, __be_, m_eject ));
        m_fsm.AddTransition(new WeaponTransition(  m_start, _fin_, NULL ));
        
        //m_fsm.AddTransition(new WeaponTransition(  m_start, __ck_, m_eject, NULL, new GuardNot(new WeaponGuardCurrentChamberEmpty(m_weapon))));
        m_fsm.AddTransition(new WeaponTransition(  m_start, __ck_, m_chamber, NULL, new WeaponGuardHasAmmo(m_weapon))); // some anims do not send BE event
        m_fsm.AddTransition(new WeaponTransition(  m_start, __ck_, m_onCK)); // some anims do not send BE event
        
        m_fsm.AddTransition(new WeaponTransition(   m_ejectChamber, __ck_, m_onCK ));
        m_fsm.AddTransition(new WeaponTransition(   m_ejectChamber, _fin_, NULL ));
        m_fsm.AddTransition(new WeaponTransition(   m_eject, __ck_, m_onCK));
        m_fsm.AddTransition(new WeaponTransition(   m_eject, _fin_, NULL));
        
        m_fsm.AddTransition(new WeaponTransition(   m_onCK, _fin_, NULL));
        
        m_fsm.AddTransition(new WeaponTransition(m_chamber, _fin_, NULL));
 
        m_fsm.SetInitialState(m_start);
