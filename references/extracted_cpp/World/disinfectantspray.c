// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/disinfectantspray.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DisinfectantSpray : Edible_Base
{
    override void InitItemVariables()
    {
        super.InitItemVariables();
 
        can_this_be_combined    = true;
        m_VarLiquidType         = GetLiquidTypeInit();
