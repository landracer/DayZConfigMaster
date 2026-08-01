// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionrepairwithtoolfromhands.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// This is just a test action!
class ActionRepairWithToolFromHands: ActionSingleUseBase
{
    void ActionRepairWithToolFromHands()
    {
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
        m_Text = &quot;#repair&quot;;
