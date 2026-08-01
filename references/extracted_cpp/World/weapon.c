// Source: F:/Games/Dayz/scripts/4_world/entities/core/inherited/weapon.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class Weapon extends InventoryItemSuper
{
    override bool IsWeapon() { return true; }
    
    proto native void Synchronize();

    proto native int GetMuzzleCount();
    
    //proto native int GetMuzzleIndexFromMagazineSlot (int magazineSlotId);
    
    proto native int GetSlotFromMuzzleIndex(int muzzleIndex);
    
    proto native int GetCurrentMuzzle();
    
    proto native void SetCurrentMuzzle(int muzzleIndex);
    
    proto native int GetMuzzleModeCount(int muzzleIndex);
    proto native void SetMuzzleMode(int muzzleIndex, int modeIndex);
    proto native void SetNextMuzzleMode(int muzzleIndex);
    proto native int GetCurrentModeBurstSize(int muzzleIndex);
    proto native bool GetCurrentModeAutoFire(int muzzleIndex);
    proto native int GetCurrentMode(int muzzleIndex);
    proto native void SetCurrentMode(int muzzleIndex, int mode);
    
    proto native owned string GetCurrentModeName(int muzzleIndex);

    proto native bool CanFire(int muzzleIndex);
    proto native bool Fire(int muzzleIndex, vector pos, vector dir, vector speed);
    proto native bool DryFire(int muzzleIndex);
    proto native bool DryDryFire(int muzzleIndex);
    proto native bool EjectCasing(int muzzleIndex);
    proto native bool CreateRound(int muzzleIndex);

    proto native bool IsChamberEmpty(int muzzleIndex);
    proto native bool IsChamberFiredOut(int muzzleIndex);
    proto native bool IsChamberJammed(int muzzleIndex);
    proto native bool IsChamberEjectable(int muzzleIndex);

    proto native bool IsChamberFull(int muzzleIndex);
    bool IsChamberFullEx(int muzzleIndex)
    {
        return IsChamberFull(muzzleIndex);
