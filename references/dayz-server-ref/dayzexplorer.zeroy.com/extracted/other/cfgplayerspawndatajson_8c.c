// ======================================================================
// File: cfgplayerspawndatajson_8c_source.html
// Category: other
// ======================================================================

class PlayerSpawnJsonDataBase : Managed
{
bool IsValid()
{
return true;
}
}
class PlayerSpawnJsonData : PlayerSpawnJsonDataBase
{
ref array<ref PlayerSpawnPreset> presets;
}
class PlayerSpawnPreset : PlayerSpawnJsonDataBase
{
int spawnWeight; //spawn probability weight
string name; //optional
ref array<string> characterTypes;
ref array<ref PlayerSpawnPresetSlotData> attachmentSlotItemSets;
ref array<ref PlayerSpawnPresetDiscreteCargoSetData> discreteUnsortedItemSets;
string GetRandomCharacterType()
{
if (characterTypes && characterTypes.Count() > 0)
return characterTypes.GetRandomElement();
Debug.Log("No characterTypes defined. Falling back to &#39;default&#39; character type, or random, if undefined","n/a","n/a","PlayerSpawnPreset");
return string.Empty;
}
override bool IsValid()
{
if (!super.IsValid())
return false;
if (spawnWeight < 1)
{
Debug.Log("Invalid spawn weight, skipping preset: " + name,"n/a","Validation","PlayerSpawnPreset");
return false;
}
return true;
}
bool HasAttachmentSlotSetsDefined()
{
return attachmentSlotItemSets && attachmentSlotItemSets.Count() > 0;
}
bool HasDiscreteUnsortedItemSetsDefined()
{
return discreteUnsortedItemSets && discreteUnsortedItemSets.Count() > 0;
}
}
class PlayerSpawnPresetSlotData : PlayerSpawnJsonDataBase
{
string slotName;
ref array<ref PlayerSpawnPresetDiscreteItemSetSlotData> discreteItemSets;
bool TranslateAndValidateSlot(EntityAI parent, inout int slotID)
{
string tmp = slotName;
if (slotName == "shoulderL")
{
tmp = "Shoulder";
}
else if (slotName == "shoulderR")
{
tmp = "Melee";
}
slotID = InventorySlots.GetSlotIdFromString(tmp);
if (!InventorySlots.IsSlotIdValid(slotID))
{
Debug.Log("Wrong slot name used: " + slotName,"n/a","Validation","PlayerSpawnPresetSlotData");
return false;
}
if (!parent)
{
Debug.Log("No parent entity found when trying to populate slot: " + slotName,"n/a","Validation","PlayerSpawnPresetSlotData");
return false;
}
if (!parent.GetInventory().HasAttachmentSlot(slotID))
{
Debug.Log("Slot: " + slotName + " undefined on entity: " + parent.GetType(),"n/a","Validation","PlayerSpawnPresetSlotData");
return false;
}
return true;
}
override bool IsValid()
{
if (!super.IsValid())
return false;
if (discreteItemSets == null || discreteItemSets.Count() < 1)
{
Debug.Log("discreteItemSets for slot: " + slotName + " undefined","n/a","Validation","PlayerSpawnPresetSlotData");
return false;
}
return true;
}
}
class PlayerSpawnPresetItemSetBase : PlayerSpawnJsonDataBase
{
bool simpleChildrenUseDefaultAttributes;
ref PlayerSpawnAttributesData attributes;
ref array<ref PlayerSpawnPresetComplexChildrenType> complexChildrenTypes;
ref array<string> simpleChildrenTypes;
int GetQuickbarIdx()
{
return -1;
}
}
//base for DISCRETE item sets
class PlayerSpawnPresetDiscreteItemSetBase : PlayerSpawnPresetItemSetBase
{
int spawnWeight;
override bool IsValid()
{
if (!super.IsValid())
return false;
if (spawnWeight < 1)
{
Debug.Log("Invalid spawnWeight set for a discrete item set!","n/a","Validation","PlayerSpawnPresetDiscreteItemSetBase");
return false;
}
return true;
}
}
class PlayerSpawnPresetDiscreteItemSetSlotData : PlayerSpawnPresetDiscreteItemSetBase
{
string itemType;
int quickBarSlot;
override bool IsValid()
{
if (!super.IsValid())
return false;
//empty &#39;itemType&#39; is valid alternative here
if (!attributes)
{
Debug.Log("No attributes defined for a discrete item set!","n/a","Validation","PlayerSpawnPresetDiscreteItemSetSlotData");
return false;
}
//unable to verify any of the other integers, since they always default to &#39;0&#39;. Needs to be configured carefully!
return true;
}
override int GetQuickbarIdx()
{
return quickBarSlot;
}
}
class PlayerSpawnPresetDiscreteCargoSetData : PlayerSpawnPresetDiscreteItemSetBase
{
string name;
}
class PlayerSpawnPresetComplexChildrenType : PlayerSpawnPresetItemSetBase
{
string itemType;
int quickBarSlot;
override bool IsValid()
{
if (!super.IsValid())
return false;
return itemType != string.Empty; //needs item type to function
}
override int GetQuickbarIdx()
{
return quickBarSlot;
}
}
class PlayerSpawnAttributesData : PlayerSpawnJsonDataBase
{
float healthMin;
float healthMax;
float quantityMin;
float quantityMax;
//ref array<string> magazineAmmoOrdered;
}