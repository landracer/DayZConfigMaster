// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/helmetbase/mich2001helmet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Mich2001Helmet extends HelmetBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionTurnOnHelmetFlashlight); //use default light actions instead?
        AddAction(ActionTurnOffHelmetFlashlight);
        AddAction(ActionToggleNVG);
