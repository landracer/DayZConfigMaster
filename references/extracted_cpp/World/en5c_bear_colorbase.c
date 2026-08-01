// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/container_base/en5c_bear_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Bear_ColorBase extends Container_Base
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo(parent) ) {return false;}  
        //better use something like super.GetName()
        if ( parent && (parent.IsKindOf(&quot;Bear_ColorBase&quot;)) )
        {
            return false;
        }
        
        return true;
