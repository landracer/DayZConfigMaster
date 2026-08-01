// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/riflechambering.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RifleChambering extends WeaponStateBase
{
    WeaponActions m_action;
    int m_actionType;
    Magazine m_srcMagazine; 
 
    ref WeaponStateBase m_start;
    ref WeaponEjectCasing m_eject;
    ref WeaponChambering_Cartridge m_chamber;
    ref WeaponChambering_W4T m_w4t;
 
    void RifleChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_action = action;
        m_actionType = actionType;
 
        // setup nested state machine
        m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_actionType);
        m_eject = new WeaponEjectCasing(m_weapon, this);
        m_chamber = new WeaponChambering_Cartridge(m_weapon, this);
        m_w4t = new WeaponChambering_W4T(m_weapon, this);
        // events
        WeaponEventAnimBulletEject __be_ = new WeaponEventAnimBulletEject;
        WeaponEventAnimBulletShow  __bs_ = new WeaponEventAnimBulletShow;
        WeaponEventAnimBulletInChamber __bc_ = new WeaponEventAnimBulletInChamber;
        WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
 
        m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
        m_fsm.AddTransition(new WeaponTransition(m_start  , __be_, m_eject));
        m_fsm.AddTransition(new WeaponTransition(m_eject  , __bs_, m_chamber));
        m_fsm.AddTransition(new WeaponTransition(m_chamber, __bc_, m_w4t));
        m_fsm.AddTransition(new WeaponTransition(m_w4t    , _fin_, NULL));
        
        // Safety exits
        m_fsm.AddTransition(new WeaponTransition(m_chamber, _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_eject  , _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_start  , _fin_, null));  
 
        m_fsm.SetInitialState(m_start);
