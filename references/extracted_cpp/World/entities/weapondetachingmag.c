// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weapondetachingmag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
// detach magazine composite state
class WeaponDetachingMag_1 extends WeaponStartAction
{ };
 
class WeaponDetachingMag_Store extends WeaponStateBase
{
    Magazine m_magazine; 
    ref InventoryLocation m_dst;
 
    override void OnEntry (WeaponEventBase e)
    {
        //if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot;  WeaponDetachingMag_Store, Detaching mag=&quot; + m_magazine.ToString() +  &quot;to loc=&quot; + InventoryLocation.DumpToStringNullSafe(m_dst)); }
        super.OnEntry(e);
        if (e)
        {
            if (!m_magazine || !m_dst)
            {
                Error(&quot;[wpnfsm] &quot; + Object.GetDebugName(m_weapon) + &quot; WeaponDetachingMag_Store, error - no magazine to load from (m_magazine=NULL)&quot;);
            }
        }
