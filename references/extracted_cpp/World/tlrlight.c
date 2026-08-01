// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/switchable_base/tlrlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TLRLight extends Switchable_Base
{
    PistollightLight    m_Light;
    
    static int          REFLECTOR_ID = 1;
    static int          GLASS_ID = 0;
    
    static string       LIGHT_OFF_GLASS = &quot;dz\\gear\\tools\\data\\flashlight_glass.rvmat&quot;;
    static string       LIGHT_OFF_REFLECTOR = &quot;dz\\weapons\\attachments\\data\\TLS3.rvmat&quot;;
    static string       LIGHT_ON_GLASS = &quot;dz\\gear\\tools\\data\\flashlight_glass_on.rvmat&quot;;
    static string       LIGHT_ON_REFLECTOR = &quot;dz\\weapons\\attachments\\data\\TLS3_on.rvmat&quot;;
    
    override ScriptedLightBase GetLight()
    {
        return m_Light;
