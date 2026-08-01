// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousBase: CABase
{           
    override bool IsContinuousAction() //informs command callback whether action is looped or oneshot
    {
        return true;
