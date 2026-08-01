// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondummyrelease.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//dummy action for release indication
class ActionDummyContinuousRelease: ActionContinuousBase
{
    void ActionDummyContinuousRelease()
    {
        m_Text = &quot;#STR_FishingAction_Yank&quot; + &quot; [&quot; + &quot;#keybind_release&quot; + &quot;]&quot;;
