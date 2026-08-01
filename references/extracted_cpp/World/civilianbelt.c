// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/civilianbelt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CivilianBelt: Belt_Base
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo( parent ) )
        {
            return false;
        }
        
        return IsEmpty();
