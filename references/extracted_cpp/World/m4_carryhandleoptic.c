// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/m4_carryhandleoptic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M4_CarryHandleOptic extends ItemOptics
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        if ( parent.IsKindOf(&quot;M4A1_Base&quot;) )
        {
            return true;
        }
 
        return false;
