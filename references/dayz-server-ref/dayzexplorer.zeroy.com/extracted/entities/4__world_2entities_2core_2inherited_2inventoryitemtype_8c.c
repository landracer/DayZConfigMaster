// ======================================================================
// File: 4__world_2entities_2core_2inherited_2inventoryitemtype_8c_source.html
// Category: entities
// ======================================================================

class CarWheelType : ItemBaseType
{
};
class ClothingType : ItemBaseType
{
};
class ItemWatchType : ItemBaseType
{
};
class ItemRadioType : ItemBaseType
{
};
class ItemTransmitterType : ClothingType
{
};
class ItemMegaphoneType : ItemBaseType
{
};
class ItemCompassType : ItemBaseType
{
};
class ItemGPSType : ItemBaseType
{
};
class ItemMapType : ItemBaseType
{
};
class ItemBookType : ItemBaseType
{
};
class ItemOpticsType : ItemBaseType
{
/*
Optics2D or in other words the model provided by `modelOptics` when `useModelOptics` is
shared for all instances of a `ItemOpticsType`.
Call the following methods during `ItemOptics.OnDrawOptics2D` to manipulate currently viewed optics.
*/
proto native void SetOptics2DTexture(int index, string textureName);
proto native void SetOptics2DMaterial(int index, string materialName);
proto native int FindOptics2DSelection(string selectionName, bool ignoreCase = true);
proto native bool IsUsingOptics2DModel();
};
class ItemBarrelType : ItemBaseType
{
};
class ItemSuppressorType : ItemBaseType
{
};
class ItemGrenadeType : ItemBaseType
{
};
class WeaponType : ItemBaseType
{
};
class UnderSlugLauncherType : WeaponType
{
};
class MagazineType : ItemBaseType
{
};