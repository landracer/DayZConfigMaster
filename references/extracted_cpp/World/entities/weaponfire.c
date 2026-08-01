// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponfire.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// fire
class WeaponDryFire extends WeaponStartAction
{
    float m_dtAccumulator;
 
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        if (e)
        {
            m_dtAccumulator = 0;
        }
        m_weapon.ResetBurstCount();
