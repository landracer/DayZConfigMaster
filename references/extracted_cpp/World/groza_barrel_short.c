// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/groza_barrel_short.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Groza_Barrel_Short extends SuppressorBase
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        if ( !parent.FindAttachmentBySlotName(&quot;weaponButtstockAK&quot;) || !parent.FindAttachmentBySlotName(&quot;weaponButtstockAK&quot;).IsKindOf(&quot;GrozaGL_LowerReceiver&quot;) )
        {
            return true;
        }
 
        return false;
