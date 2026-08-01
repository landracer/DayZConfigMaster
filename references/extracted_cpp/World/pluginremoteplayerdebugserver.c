// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/pluginremoteplayerdebugserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginRemotePlayerDebugServer extends PluginBase
{   
    ref set<PlayerBase> m_ClientList = new set<PlayerBase>;
    
    ref array<ref RemotePlayerStatDebug> m_PlayerDebugStats = new array<ref RemotePlayerStatDebug>;
    ref map<PlayerBase, ref RemotePlayerDamageDebug> m_PlayerDebugDamage = new map<PlayerBase, ref RemotePlayerDamageDebug>;
    float m_AccuTime;
    const int INTERVAL = 1;
    
    eRemoteDebugType m_DebugType;
    bool m_Watching;
    
    override void OnUpdate(float delta_time)
    {
        #ifdef SERVER
        if ( m_ClientList.Count() != 0 )
        {
            m_AccuTime += delta_time;
            
            if ( m_AccuTime > INTERVAL )
            {
                m_AccuTime = 0;
                SendDebug();
            }
        }
        #endif
