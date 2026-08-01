// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamageloopeddeferred.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AreaDamageLoopedDeferred : AreaDamageLooped
{
    protected float m_DeferDuration = 1.0;
 
    override void SetDeferDuration(float time)
    {
        m_DeferDuration = time;
