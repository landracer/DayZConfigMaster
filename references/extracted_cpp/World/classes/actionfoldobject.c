// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionfoldobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFoldObject: ActionInteractBase
{
    void ActionFoldObject()
    {
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_MEDIUM;
        m_Text = &quot;#fold&quot;;
