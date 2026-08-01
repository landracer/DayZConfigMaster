// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/leatherbelt_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LeatherBelt_ColorBase extends Belt_Base
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo( parent ) )
        {
            return false;
        }
        
        return IsEmpty();
