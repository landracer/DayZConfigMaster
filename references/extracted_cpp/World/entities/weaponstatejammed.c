// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponstatejammed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//Obsolete
class WeaponStateJammed extends WeaponStableState
{
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        m_weapon.SetJammed(true);
