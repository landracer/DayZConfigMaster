// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/bullethide.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BulletHide extends WeaponStateBase
{
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if(e)
        {
            m_weapon.SelectionBulletHide();
            m_weapon.EffectBulletHide(m_weapon.GetCurrentMuzzle());
        }
