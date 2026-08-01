// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionattachwheels.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionAttachWheels: ActionAttach
{
    int m_wheel_hub;
    
    void ActionAttachWheels()
    {
        m_wheel_hub = -1;
        m_Text = &quot;#attach&quot;;
