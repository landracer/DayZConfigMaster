// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/roadflare.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    Author: Boris Vacula
    Description: The flare has 3 burning states during which it uses different particle effects while illumnatingthe environment. After it burns up, it still releases smoke for a while.
    When the flare is dropped while its burning, it is stood up on its stands. This makes the shadows, illumination and particles stand out better.
*/
 
enum RoadflareBurningState
{
    NOT_BURNING,
    INITIAL_BURN,
    MAIN_BURN,
    FINAL_BURN,
    SMOKE_ONLY
