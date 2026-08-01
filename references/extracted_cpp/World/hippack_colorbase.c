// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/hippack_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HipPack_ColorBase : Belt_Base 
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo( parent ) )
        {
            return false;
        }
        
        return IsEmpty();
