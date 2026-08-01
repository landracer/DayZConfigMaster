// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/bulletshow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BulletShow extends WeaponStateBase
{
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if(e)
            m_weapon.ShowBullet(0);
