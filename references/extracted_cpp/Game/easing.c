// Source: F:/Games/Dayz/scripts/3_game/static/easing.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class Easing
{
    static float EaseInSine( float t ) 
    {
        return -1 * Math.Cos( t * ( Math.PI / 2 ) ) + 1;
