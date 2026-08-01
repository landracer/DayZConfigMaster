// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementaiming.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/* 
DayZPlayerImplement
 
this file is implemenation of dayzPlayer in script 
- logic of movement 
- camera switching logic
 
*/
 
enum eSwayStates
{
    DEFAULT,
    HOLDBREATH_IN,
    HOLDBREATH_STABLE,
    HOLDBREATH_EXHAUSTED,
    MAX
