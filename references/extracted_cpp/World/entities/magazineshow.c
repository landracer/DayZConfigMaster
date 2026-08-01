// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/magazineshow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MagazineShow extends WeaponStateBase
{
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if (e)
            m_weapon.ShowMagazine();
