// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponreplacingmagandchambernext.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class DetachOldMagazine extends WeaponStateBase
{
    Magazine m_oldMagazine; 
    ref InventoryLocation m_newDst;
 
    void DetachOldMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
    {
        m_oldMagazine = NULL;
        m_newDst = NULL;
