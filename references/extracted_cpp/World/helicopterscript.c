// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/helicopterscript.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HelicopterScript extends HelicopterAuto
{
    void HelicopterScript()
    {
        SetEventMask(EntityEvent.POSTSIMULATE);
