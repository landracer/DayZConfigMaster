// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionforceasip.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceASip: ActionForceABite
{
    void ActionForceASip()
    {
        m_Sounds.Insert(&quot;DrinkBottle_0&quot;);
        m_Sounds.Insert(&quot;DrinkBottle_1&quot;);
        m_Text = &quot;#give_a_sip&quot;;
