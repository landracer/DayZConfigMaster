// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponchamberinglooped.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// load x bullets
class LoopedChambering_EndLoop extends WeaponStartAction
{
    override bool IsWaitingForActionFinish () { return true; }
};
 
class LoopedChambering_Wait4ShowBullet2 extends WeaponStateBase
{ 
    override bool IsWaitingForActionFinish () { return false; }
};
 
class LoopedChambering extends WeaponStateBase
{
    WeaponActions m_action;
    int m_startActionType;
    int m_endActionType;
    Magazine m_srcMagazine; 
    ref InventoryLocation m_srcMagazinePrevLocation;
 
    ref WeaponStateBase m_start;
    ref WeaponEjectCasing m_eject;
    ref WeaponChambering_Base m_chamber;
    ref LoopedChambering_Wait4ShowBullet2 m_w4sb2;
    ref WeaponStartAction m_endLoop;
    ref BulletHide_W4T m_hideB;
 
    void LoopedChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int startActionType = -1, int endActionType = -1)
    {
        m_action = action;
        m_startActionType = startActionType;
        m_endActionType = endActionType;
 
        // setup nested state machine
        m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_startActionType);
        m_eject = new WeaponEjectCasing(m_weapon, this);
        m_chamber = new WeaponChambering_Cartridge_ChambToMag(m_weapon, this);
        m_w4sb2 = new LoopedChambering_Wait4ShowBullet2(m_weapon, this);
        m_hideB = new BulletHide_W4T(m_weapon, this);
        m_endLoop = new LoopedChambering_EndLoop(m_weapon, this, m_action, m_endActionType); // @NOTE: termination playing action - dummy?
        // events
        WeaponEventBase                         _fin_ = new WeaponEventHumanCommandActionFinished;
        WeaponEventContinuousLoadBulletStart    __lS_ = new WeaponEventContinuousLoadBulletStart;
        WeaponEventContinuousLoadBulletEnd      __lE_ = new WeaponEventContinuousLoadBulletEnd;
        WeaponEventAnimBulletShow               __bs_ = new WeaponEventAnimBulletShow;
        WeaponEventAnimBulletHide               __bh_ = new WeaponEventAnimBulletHide;
        WeaponEventAnimBulletEject              __be_ = new WeaponEventAnimBulletEject;
        WeaponEventAnimBulletInMagazine         __bM_ = new WeaponEventAnimBulletInMagazine;
        WeaponEventAnimBulletShow2              _bs2_ = new WeaponEventAnimBulletShow2;
 
        m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
        m_fsm.AddTransition(new WeaponTransition(m_start,   __be_, m_eject));
        m_fsm.AddTransition(new WeaponTransition(m_start,   __bs_, m_chamber));
        m_fsm.AddTransition(new WeaponTransition(m_eject,   __bs_, m_chamber));
        
 
        m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb2, NULL, new GuardAnd(new GuardAnd(new WeaponGuardHasAmmoInLoopedState(m_chamber), new WeaponGuardInternalMagazineHasRoomForBullet(m_weapon)),new WeaponGuardWeaponManagerWantContinue())));
        m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_endLoop));
        m_fsm.AddTransition(new WeaponTransition(m_w4sb2,   __bh_, m_hideB));
        m_fsm.AddTransition(new WeaponTransition(m_hideB,   __bs_, m_chamber));
 
        m_fsm.AddTransition(new WeaponTransition(m_endLoop, _fin_, NULL));
        
        // Safety exits
        m_fsm.AddTransition(new WeaponTransition(m_hideB,   _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_w4sb2,   _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_chamber, _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_eject  , _fin_, null));
        m_fsm.AddTransition(new WeaponTransition(m_start  , _fin_, null));          
 
        m_fsm.SetInitialState(m_start);
