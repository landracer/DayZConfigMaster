// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/headtorch_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Headtorch_ColorBase extends Clothing
{
    HeadtorchLight m_Light;
    
    static int      REFLECTOR_ID = 4;
    static int      GLASS_ID = 5;
    
    static string   LIGHT_OFF_GLASS         = &quot;dz\\characters\\headgear\\data\\HeadTorchGlass.rvmat&quot;;
    static string   LIGHT_OFF_REFLECTOR     = &quot;dz\\characters\\headgear\\data\\HeadTorch.rvmat&quot;;
    static string   LIGHT_ON_GLASS          = &quot;dz\\characters\\headgear\\data\\HeadTorchGlass_on.rvmat&quot;;
    static string   LIGHT_ON_GLASS_RED      = &quot;dz\\characters\\headgear\\data\\HeadTorchGlass_on_red.rvmat&quot;;
    static string   LIGHT_ON_REFLECTOR      = &quot;dz\\characters\\headgear\\data\\HeadTorch_ON.rvmat&quot;;
    static string   LIGHT_ON_REFLECTOR_RED  = &quot;dz\\characters\\headgear\\data\\HeadTorch_ON_red.rvmat&quot;;
    
    static vector m_OnHeadLocalPos = Vector(0.12,0.15,0);
    static vector m_OnHeadLocalOri = Vector(0,90,0);
    static string m_OffHeadLightPoint = &quot;beamStart&quot;;
    static string m_OffHeadLightTarget = &quot;beamEnd&quot;;
    
    ref Timer m_Timer;
    
    void Headtorch_ColorBase()
    {
        if (g_Game.IsServer())
        {
            if (!m_Timer)
                m_Timer = new Timer(CALL_CATEGORY_SYSTEM);
            
            m_Timer.Run(1 , this, &quot;CheckParent&quot;, NULL, false);
        }
