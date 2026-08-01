// Source: F:/Games/Dayz/scripts/4_world/entities/building/specific/land_buoy.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Buoy extends House
{
    protected BuoyLight m_Light;
    
    override void DeferredInit()
    {
        super.DeferredInit();
        
        if (!g_Game.IsDedicatedServer())
        {
            m_Light = BuoyLight.Cast(ScriptedLightBase.CreateLightAtObjMemoryPoint(BuoyLight, this, &quot;light_01&quot;));
            m_Light.SetSelectionID(GetHiddenSelectionIndex(&quot;Glass&quot;));
            m_Light.UpdateLightSourceMaterial(BuoyLight.LIGHT_MAT_ON);
        }
