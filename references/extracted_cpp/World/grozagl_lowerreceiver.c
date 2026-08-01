// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/grozagl_lowerreceiver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GrozaGL_LowerReceiver extends ItemBase
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        if ( parent.FindAttachmentBySlotName(&quot;weaponMuzzleAK&quot;) == NULL )
        {
            return true;
        }
        return true;
