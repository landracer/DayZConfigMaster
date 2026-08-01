// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/atlasbipod.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AtlasBipod extends Inventory_Base
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        bool m4_hndgrd = false;
        bool ak_hndgrd = false;
 
        if ( parent.FindAttachmentBySlotName(&quot;weaponHandguardM4&quot;) != NULL )
        {
            m4_hndgrd = parent.FindAttachmentBySlotName(&quot;weaponHandguardM4&quot;).IsKindOf(&quot;M4_RISHndgrd&quot;);
        }
        
        if ( parent.FindAttachmentBySlotName(&quot;weaponHandguardAK&quot;) != NULL )
        {
            ak_hndgrd = parent.FindAttachmentBySlotName(&quot;weaponHandguardAK&quot;).IsKindOf(&quot;AK_RailHndgrd&quot;);
        }
 
        if ( m4_hndgrd || ak_hndgrd )
        {
            return true;
        }
        
        return false;
