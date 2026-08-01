// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionremoveseed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRemoveSeed: ActionInteractBase
{
    SeedBase m_Seed;
    
    void ActionRemoveSeed()
    {
        m_Text = &quot;#take&quot;;
