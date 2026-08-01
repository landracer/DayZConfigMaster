// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/makarovpbsuppressor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MakarovPBSuppressor extends ItemSuppressor
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        if ( parent.FindAttachmentBySlotName(&quot;suppressorImpro&quot;) == NULL && parent.FindAttachmentBySlotName(&quot;weaponMuzzleMP5&quot;) == NULL )
        {
            return true;
        }
        return false;
