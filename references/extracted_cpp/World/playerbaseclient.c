// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/playerbaseclient.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerBaseClient extends PlayerBase
{   
    static ScriptedLightBase    m_PersonalLight;
    static bool                 m_PersonalLightEnabledOnCurrentServer = false; // &quot;disablePersonalLight&quot; in server.cfg decides if this is true or false
    static bool                 m_PersonalLightDisabledByDebug = false;
    static bool                 m_PersonalLightIsSwitchedOn = true;
    
    static void CreatePersonalLight()
    {
        if (!m_PersonalLight && ( !g_Game.IsServer() || !g_Game.IsMultiplayer() ))
        {
            m_PersonalLight = ScriptedLightBase.CreateLight(PersonalLight, &quot;0 0 0&quot;);
        }
