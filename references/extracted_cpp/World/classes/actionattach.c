// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionattach.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachActionData : ActionData
{
    int m_AttSlot;
}
 
 
class ActionAttach : ActionSingleUseBase
{
    void ActionAttach()
    {
        m_Text = &quot;#attach&quot;;
