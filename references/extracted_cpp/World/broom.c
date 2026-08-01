// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/broom.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BroomBase : FlammableBase
{
    override void Init()
    {
        super.Init();
        m_DecraftResult = &quot;LongWoodenStick&quot;;
        m_ParticleLocalPos = Vector(0, 1.2, 0);
