// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/leatherhat_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LeatherHat_ColorBase extends HeadGear_Base
{
    /*override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        bool is_mask_only = false;
        
        if ( parent.FindAttachmentBySlotName( &quot;Mask&quot; ) )
        {
            is_mask_only = parent.FindAttachmentBySlotName( &quot;Mask&quot; ).ConfigGetBool( &quot;noHelmet&quot; );
        }
        
        if ( ( GetNumberOfItems() == 0 || !parent || parent.IsMan() ) && !is_mask_only )
        {
            return true;
        }
        return false;
    }*/
}
 
class LeatherHat_Natural extends LeatherHat_ColorBase {};
class LeatherHat_Beige extends LeatherHat_ColorBase {};
class LeatherHat_Brown extends LeatherHat_ColorBase {};
class LeatherHat_Black extends LeatherHat_ColorBase {};
