// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/disinfectantalcohol.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DisinfectantAlcohol : Edible_Base
{
    
    override void InitItemVariables()
    {
        super.InitItemVariables();
 
        can_this_be_combined    = true;
        m_VarLiquidType         = GetLiquidTypeInit();
