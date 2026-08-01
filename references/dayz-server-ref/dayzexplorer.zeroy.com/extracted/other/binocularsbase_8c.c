// ======================================================================
// File: binocularsbase_8c_source.html
// Category: other
// ======================================================================

class BinocularsBase extends Weapon
{
override int GetSlotsCountCorrect()
{
return GetInventory().GetAttachmentSlotsCount();
};
}