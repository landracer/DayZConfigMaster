// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/spheretrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class SphereTrigger : Trigger
{
    override void EOnInit(IEntity other, int extra)
    {
        SetCollisionSphere(3);
