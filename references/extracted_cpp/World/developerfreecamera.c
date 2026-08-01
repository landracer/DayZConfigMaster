// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/developerfreecamera.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DeveloperFreeCamera
{
    static void OnInit();
    
    static void FreeCameraToggle(PlayerBase player, bool teleport_player = false)
    {
        if (!IsFreeCameraEnabled())
        {
            EnableFreeCamera(player);
        }
        else 
        {
            DisableFreeCamera(player, teleport_player);
        }
