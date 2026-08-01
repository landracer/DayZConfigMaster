// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercamera_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy


float   Limit(float pV, float pMin, float pMax)
{
    if (pV >= pMin && pV <= pMax)
    {
        return pV;
    }
    else if (pV < pMin)
    {
        return pMin;
    }
    else
    {
        return pMax;
    }   
