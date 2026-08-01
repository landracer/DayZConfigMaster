// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioninput.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum ActionInputType
{
    AIT_CONTINUOUS,             //React to hold input and release after it
    AIT_SINGLE,                 //React to click input - single use actions
    AIT_DOUBLECLICK,            //React to double click input - single use actions
    AIT_HOLDSINGLE,             //React to hold input - single use actions
    AIT_CLICKCONTINUOUS,        //React to click input for start and click for end
    AIT_NOINPUTCONTROL,
    AIT_INVENTORYINPUT,         //Inventory specific actions
