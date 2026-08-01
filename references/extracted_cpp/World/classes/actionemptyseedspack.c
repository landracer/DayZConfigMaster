// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionemptyseedspack.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    User action for emptying packs of seeds (horticulture)
*/
 
 
class ActionEmptySeedsPack: ActionSingleUseBase
{
    void ActionEmptySeedsPack()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYSEEDSPACK;
        m_FullBody = false;
        m_Text = &quot;#unbox&quot;;
