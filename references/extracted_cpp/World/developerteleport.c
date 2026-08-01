// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/developerteleport.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DeveloperTeleport
{
    protected static const float TELEPORT_DISTANCE_MAX = 1000;
    
    static void TeleportAtCursor()
    {
        if ( !DeveloperFreeCamera.IsFreeCameraEnabled() )
        {
            PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
            vector pos_player = player.GetPosition();
            
            Object ignore;
            if (!Class.CastTo(ignore, player.GetParent()))
            {
                ignore = player;
            }
 
            vector rayStart = g_Game.GetCurrentCameraPosition();
            vector rayEnd = rayStart + g_Game.GetCurrentCameraDirection() * 1000;       
            vector hitPos;
            vector hitNormal;
            int hitComponentIndex;      
            DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, ignore);
                
            float distance = vector.Distance( pos_player, hitPos );
            
            if ( distance < TELEPORT_DISTANCE_MAX )
            {
                bool breakSync = false;
                
                #ifdef DIAG_DEVELOPER
                breakSync = DiagMenu.GetBool(DiagMenuIDs.MISC_TELEPORT_BREAKS_SYNC);
                #endif
                
                SetPlayerPosition(player, hitPos, breakSync);
            }
            else
            {
                Debug.LogWarning(&quot;Distance for teleportation is too far!&quot;);
            }               
        }
