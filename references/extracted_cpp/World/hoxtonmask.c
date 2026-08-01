// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/hoxtonmask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HoxtonMask extends ClothingBase
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        bool headgear_present = false;
        
        if ( parent.FindAttachmentBySlotName( &quot;Headgear&quot; ) )
        {
            headgear_present = parent.FindAttachmentBySlotName( &quot;Headgear&quot; ).ConfigGetBool( &quot;noMask&quot; );
        }
        
        if ( ( GetNumberOfItems() == 0 || !parent || parent.IsMan() ) && !headgear_present )
        {
            return true;
        }
        return false;
