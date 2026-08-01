// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/petrollighter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PetrolLighter extends ItemBase
{
    override bool CanIgniteItem( EntityAI ignite_target = NULL )
    {
        if ( GetQuantity() > 0 )
            return true;
        else
            return false;
