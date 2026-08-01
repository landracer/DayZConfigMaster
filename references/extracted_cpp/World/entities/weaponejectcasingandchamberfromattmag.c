// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponejectcasingandchamberfromattmag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class WeaponEjectCasing extends WeaponStateBase
{
    void WeaponEjectCasing (Weapon_Base w = NULL, WeaponStateBase parent = NULL) { }
 
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if (e)
        {
            if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot; ejected fired out casing&quot;); }
            int mi = m_weapon.GetCurrentMuzzle();
            if(m_weapon.IsChamberFiredOut(mi))
            {
                m_weapon.EjectCasing(mi);
            }
            m_weapon.EffectBulletHide(mi);
            m_weapon.SelectionBulletHide();
        }
