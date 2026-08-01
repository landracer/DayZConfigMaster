// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/binocularsbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BinocularsBase extends Weapon
{   
    override int GetSlotsCountCorrect()
    {
        return GetInventory().GetAttachmentSlotsCount();
