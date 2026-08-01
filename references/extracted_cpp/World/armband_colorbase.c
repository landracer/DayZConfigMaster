// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/armband_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Armband_ColorBase extends Clothing
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo(parent) ) {return false;}
        if ( GetNumberOfItems() == 0 || !parent || parent.IsMan() )
        {
            return true;
        }
        return false;
