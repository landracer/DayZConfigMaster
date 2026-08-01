// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/cylindertrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class CylinderTrigger : Trigger
{
    override void EOnInit(IEntity other, int extra)
    {
        SetCollisionCylinderTwoWay(1, -0.25, 0.25);
