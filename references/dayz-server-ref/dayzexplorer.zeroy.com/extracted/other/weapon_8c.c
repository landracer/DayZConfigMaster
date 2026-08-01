// ======================================================================
// File: weapon_8c_source.html
// Category: other
// ======================================================================

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
}
proto native bool HasInternalMagazine(int muzzleIndex);
proto native int GetInternalMagazineCartridgeCount(int muzzleIndex);
proto native int GetInternalMagazineMaxCartridgeCount(int muzzleIndex);
bool IsInternalMagazineFull(int muzzleIndex) { return GetInternalMagazineMaxCartridgeCount(muzzleIndex) == GetInternalMagazineCartridgeCount(muzzleIndex); }
proto native int GetTotalCartridgeCount(int muzzleIndex);
proto native int GetTotalMaxCartridgeCount(int muzzleIndex);
//proto native int GetChamberAmmoTypeCount(int muzzleIndex);
proto native owned string GetRandomChamberableAmmoTypeName(int muzzleIndex);
proto native owned string GetChamberAmmoTypeName(int muzzleIndex);
proto bool GetCartridgeInfo(int muzzleIndex, out float ammoDamage, out string ammoTypeName);
proto bool GetInternalMagazineCartridgeInfo(int muzzleIndex, int cartridgeIndex, out float ammoDamage, out string ammoTypeName);
proto bool PopCartridgeFromChamber(int muzzleIndex, out float ammoDamage, out string ammoTypeName);
proto native bool PushCartridgeToChamber(int muzzleIndex, float ammoDamage, string ammoTypeName);
proto native void EffectBulletShow(int muzzleIndex, float dmg, string type);
proto native void EffectBulletHide(int muzzleIndex);
proto native void SetGroundAnimFrameIndex(int groundAnimFrameIndex);
proto bool PopCartridgeFromInternalMagazine(int muzzleIndex, out float ammoDamage, out string ammoTypeName);
proto native bool PushCartridgeToInternalMagazine(int muzzleIndex, float ammoDamage, string ammoTypeName);
proto native bool CanAttachMagazine(int muzzleIndex, Magazine mag);
proto native bool TestAttachMagazine(int muzzleIndex, Magazine mag, bool do_occupancy_test, bool do_script_check);
proto native bool AttachMagazine(int muzzleIndex, Magazine mag);
proto native Magazine GetMagazine(int muzzleIndex);
proto native float GetReloadTime(int muzzleIndex);
proto native bool CanChamberFromMag(int muzzleIndex, Magazine mag);
proto native owned string GetChamberedCartridgeMagazineTypeName(int muzzleIndex);
proto native owned string GetRandomMagazineTypeName(int muzzleIndex);
proto native owned string GetMaxMagazineTypeName(int muzzleIndex);
proto native int GetMagazineTypeCount(int muzzleIndex);
proto native bool EnterOptics();
proto native bool IsInOptics();
proto native bool ExitOptics();
proto native float GetZoom();
proto native bool SetZoom(float percent);
proto native bool ZoomIn();
proto native bool ZoomOut();
proto native int GetStepZoom();
proto native bool SetStepZoom(int step);
proto native bool StepZoomIn();
proto native bool StepZoomOut();
proto native int GetStepZeroing(int muzzleIndex = 0);
proto native bool SetStepZeroing(int muzzleIndex, int step);
proto native bool StepZeroingUp(int muzzleIndex = 0);
proto native bool StepZeroingDown(int muzzleIndex = 0);
proto native float GetCurrentZeroing(int muzzleIndex = 0);
proto native float GetZeroingClamp(int muzzleIndex = 0);
proto native bool FlashlightOn();
proto native bool FlashlightOff();
proto native bool IsFlashlightOn();
proto native UnderSlugLauncher GetAttachedLauncher();
proto native ItemOptics GetAttachedOptics();
proto native ItemSuppressor GetAttachedSuppressor();
proto native void GetCameraPoint(int muzzleIndex, out vector pos, out vector dir);
proto native float GetZoomInit(int muzzleIndex);
proto native float GetZoomMin(int muzzleIndex);
proto native float GetZoomMax(int muzzleIndex);
proto native float GetZeroingDistanceZoomMin(int muzzleIndex);
proto native float GetZeroingDistanceZoomMax(int muzzleIndex);
proto native void SelectionBulletShow();
proto native void SelectionBulletHide();
proto native void SelectionMagazineShow();
proto native void SelectionMagazineHide();
void StepZeroingUpAllMuzzles()
{
for (int i = 0; i < GetMuzzleCount(); i++)
{
StepZeroingUp(i);
}
}
void StepZeroingDownAllMuzzles()
{
for (int i = 0; i < GetMuzzleCount(); i++)
{
StepZeroingDown(i);
}
}
};