// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/ropebelt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RopeBelt: Belt_Base
{       
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionDeCraftRopeBelt);
