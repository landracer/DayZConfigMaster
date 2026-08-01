// Source: F:/Games/Dayz/scripts/3_game/worldlighting.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WorldLighting
{
    string lighting_default = &quot;dz\\data\\lighting\\lighting_default.txt&quot;;
    string lighting_darknight = &quot;dz\\data\\lighting\\lighting_darknight.txt&quot;;
    string lighting_sakhal = &quot;DZ\\worlds\\sakhal\\data\\lighting\\lighting_sakhal.txt&quot;;
    
    void WorldLighting() {}
    void ~WorldLighting() {}
        
    // sets global lighting config by given value (sent from server, defined in server config)
    void SetGlobalLighting( int lightingID )
    {
        switch ( lightingID )
        {
            case 0:
                g_Game.GetWorld().LoadNewLightingCfg( lighting_default );
                break;
                
            case 1:
                g_Game.GetWorld().LoadNewLightingCfg( lighting_darknight );
                break;
                
            case 2:
                g_Game.GetWorld().LoadNewLightingCfg( lighting_sakhal );
                break;
        }
