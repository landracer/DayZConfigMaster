// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepairpart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
/*
TODO:
No pairing with part and tool similar to &#39;CanUseToolToBuildPart&#39; exists here, since only one type of part is damage-able at this point.
This should be handled by adding &quot;repair_action_type&quot; array to basebuilding and tool configs, this would allow for independent pairing of repair action.
*/
 
class RepairPartActionReciveData : ActionReciveData
{
    int m_ComponentIndexRecived;
