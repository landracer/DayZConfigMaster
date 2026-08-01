// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponattachmagazine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RemoveNewMagazineFromInventory extends WeaponStateBase
{
    Magazine m_newMagazine; 
    ref InventoryLocation m_newSrc;
 
    void RemoveNewMagazineFromInventory (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
    {
        m_newMagazine = NULL;
        m_newSrc = NULL;
