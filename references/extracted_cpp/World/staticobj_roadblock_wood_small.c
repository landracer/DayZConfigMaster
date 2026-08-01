// Source: F:/Games/Dayz/scripts/4_world/entities/building/wrecks/staticobj_roadblock_wood_small.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StaticObj_Roadblock_Wood_Small: House
{       
    PointLightBase m_BlinkingLight;
    
    override void EEInit()
    {
        super.EEInit();
    
        if ( !g_Game.IsDedicatedServer() )
        {
            m_BlinkingLight = EntranceLight.Cast(ScriptedLightBase.CreateLightAtObjMemoryPoint(Roadblock_Light, this, &quot;Light&quot;));
        }
