// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/punchedcard.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PunchedCard : Paper 
{
    override void SetActions()
    {
        super.SetActions();
 
        AddAction(ActionUseUndergroundPanel);
