// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/chernarusmap_open.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChernarusMap extends ItemMap
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionUnfoldMap);
