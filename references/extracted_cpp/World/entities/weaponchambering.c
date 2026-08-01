// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponchambering.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// load 1 bullet
class WeaponChambering_Start extends WeaponStartAction
{ 
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if (e)
        {
            m_weapon.EffectBulletHide(m_weapon.GetCurrentMuzzle());
            m_weapon.SelectionBulletHide();
            //m_weapon.ForceSyncSelectionState();
        }
