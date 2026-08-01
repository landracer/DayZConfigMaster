// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/painkillertablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PainkillerTablets extends Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceConsumeSingle);
        AddAction(ActionEatTabletFromWrapper);
