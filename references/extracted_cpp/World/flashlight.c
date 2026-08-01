// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/flashlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Flashlight extends ItemBase
{
    FlashlightLight m_Light;
    
    static int      REFLECTOR_ID = 1;
    static int      GLASS_ID = 2;
    
    static string   LIGHT_OFF_GLASS = &quot;dz\\gear\\tools\\data\\flashlight_glass.rvmat&quot;;
    static string   LIGHT_OFF_REFLECTOR = &quot;dz\\gear\\tools\\data\\flashlight.rvmat&quot;;
    static string   LIGHT_ON_GLASS = &quot;dz\\gear\\tools\\data\\flashlight_glass_on.rvmat&quot;;
    static string   LIGHT_ON_REFLECTOR = &quot;dz\\gear\\tools\\data\\flashlight_glass_on.rvmat&quot;;
    
    override void OnWorkStart()
    {
        if ( !g_Game.IsServer()  ||  !g_Game.IsMultiplayer() ) // Client side
        {
            m_Light = FlashlightLight.Cast(  ScriptedLightBase.CreateLight(FlashlightLight, &quot;0 0 0&quot;, 0.08)  ); // Position is zero because light is attached on parent immediately.
            m_Light.AttachOnMemoryPoint(this, &quot;beamStart&quot;, &quot;beamEnd&quot;);
        }
        
        SetObjectMaterial(GLASS_ID, LIGHT_ON_GLASS);
        SetObjectMaterial(REFLECTOR_ID, LIGHT_ON_REFLECTOR);
