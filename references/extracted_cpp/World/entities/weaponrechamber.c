// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponrechamber.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
// rechamber (== eject cartridge + load another + store the old one)
class WeaponRechamber extends WeaponStateBase
{
    int m_actionEject;
    int m_actionTypeEject;
    int m_actionLoad;
    int m_actionTypeLoad;
    Magazine m_dstMagazine; 
    Magazine m_srcMagazine; 
 
    ref WeaponEjectBullet m_eje;
    ref WeaponChambering m_loa;
 
    void WeaponRechamber (Weapon_Base w = NULL, WeaponStateBase parent = NULL, int actionEject = -1, int actionTypeEject = -1, int actionLoad = -1, int actionTypeLoad = -1)
    {
        m_actionEject = actionEject;
        m_actionTypeEject = actionTypeEject;
        m_actionLoad = actionLoad;
        m_actionTypeLoad = actionTypeLoad;
 
        // setup nested state machine
        m_eje = new WeaponEjectBullet(m_weapon, this, m_actionEject, m_actionTypeEject);
        m_loa = new WeaponChambering(m_weapon, this, m_actionLoad, m_actionTypeLoad);
        // events
        WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
 
        m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
        m_fsm.AddTransition(new WeaponTransition(m_eje, _fin_, m_loa));
        m_fsm.AddTransition(new WeaponTransition(m_loa, _fin_, NULL));
 
        m_fsm.SetInitialState(m_eje);
