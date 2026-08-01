// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionremoveplant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRemovePlant: ActionInteractBase
{
    PlantBase m_Plant;
    
    void ActionRemovePlant()
    {
        m_Text = &quot;#remove_plant&quot;;
