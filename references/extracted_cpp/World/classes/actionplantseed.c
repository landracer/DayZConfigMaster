// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionplantseed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPlantSeed : ActionSingleUseBase
{
    void ActionPlantSeed()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_DROPITEM_HANDS;
        m_FullBody      = false;
 
        m_Text = &quot;#plant_seed&quot;;
