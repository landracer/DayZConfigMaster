// ======================================================================
// File: weaponstatejammed_8c_source.html
// Category: other
// ======================================================================

//Obsolete
class WeaponStateJammed extends WeaponStableState
{
override void OnEntry (WeaponEventBase e)
{
super.OnEntry(e);
m_weapon.SetJammed(true);
}
override void OnExit (WeaponEventBase e)
{
super.OnExit(e);
//m_weapon.SetJammed(false);
}
};