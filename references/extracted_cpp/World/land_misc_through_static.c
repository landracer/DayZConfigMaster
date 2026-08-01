// Source: F:/Games/Dayz/scripts/4_world/entities/building/industrial/misc/land_misc_through_static.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Misc_Through_Static : BuildingSuper
{
    override EWaterSourceObjectType GetWaterSourceObjectType()
    {
        return EWaterSourceObjectType.THROUGH;
