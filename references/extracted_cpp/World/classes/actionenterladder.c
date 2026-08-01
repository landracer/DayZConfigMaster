// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionenterladder.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ActionEnterLadder: ActionInteractBase
{
    private const string GEOM_LOD_NAME  = LOD.NAME_GEOMETRY;
    private const string MEM_LOD_NAME   = LOD.NAME_MEMORY;  
 
    void ActionEnterLadder()
    {
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#enter_ladder&quot;;
