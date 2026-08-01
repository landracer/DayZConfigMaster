// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/charcoaltablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CharcoalTablets extends Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceConsumeSingle);
        AddAction(ActionEatTabletFromWrapper);
