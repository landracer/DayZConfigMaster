// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/switchable_base/universallight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalLight extends Switchable_Base
{
    UniversallightLight     m_Light;
    
    static int          REFLECTOR_ID = 1;
    static int          GLASS_ID = 2;
    
    static string       LIGHT_OFF_GLASS     = &quot;dz\\gear\\tools\\data\\flashlight_glass.rvmat&quot;;
    static string       LIGHT_OFF_REFLECTOR = &quot;dz\\weapons\\attachments\\data\\m4_flashlight.rvmat&quot;;
    static string       LIGHT_ON_GLASS      = &quot;dz\\gear\\tools\\data\\flashlight_glass_on.rvmat&quot;;
    static string       LIGHT_ON_REFLECTOR  = &quot;dz\\weapons\\attachments\\data\\m4_flashlight_on.rvmat&quot;;
    
    ref array<int>      m_AttachmentSlotsCheck;
    
    void UniversalLight()
    {
        InitAttachmentsSlotsToCheck(m_AttachmentSlotsCheck);
