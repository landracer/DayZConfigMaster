// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/mantrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ManTrigger : Trigger
{
    override void EOnInit(IEntity other, int extra)
    {
        SetExtents(&quot;-2 -2 -2&quot;, &quot;2 2 2&quot;);
