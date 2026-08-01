// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/switchable_base/chainsaw.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Chainsaw extends Switchable_Base
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo(parent) ) {return false;}
        if( !GetCompEM().IsSwitchedOn() )
        {
            return true;
        }
        return false;
