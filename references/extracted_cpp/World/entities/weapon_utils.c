// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/weapon_utils.c Source File
// Extracted from DayZ Code Explorer by Zeroy

bool pushToChamberFromAttachedMagazine(Weapon_Base weapon, int muzzleIndex)
{
    Magazine mag = weapon.GetMagazine(muzzleIndex);
    if (mag && !mag.IsDamageDestroyed())
    {
        if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, using attached magazine mag=&quot; + mag.ToString()); }
        float damage;
        string type;
        if (mag && mag.LocalAcquireCartridge(damage, type))
        {
            weapon.SelectionBulletShow();
            if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, ok - cartridge acquired: dmg=&quot; + damage + &quot; type=&quot; + type); }
        }
        else
            Error(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, error - cannot take cartridge from magazine&quot;);
 
        if (weapon.PushCartridgeToChamber(muzzleIndex, damage, type))
        {
            if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, ok - loaded chamber&quot;); }
            return true;
        }
        else
            Error(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, error - cannot load chamber!&quot;);
    }
    else
    {
        if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine - magazine destroyer or no attached&quot;); }
        //Error(&quot;[wpnfsm] &quot; + Object.GetDebugName(weapon) + &quot; chamberFromAttachedMagazine, error - no magazine attached&quot;);
    }
    return false;
