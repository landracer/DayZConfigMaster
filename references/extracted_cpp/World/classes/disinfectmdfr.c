// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/disinfectmdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DisinfectionMdfr: ModifierBase
{
    const int LIFETIME = 100;
    //const int EFFECT_TIME = 100;
    PluginTransmissionAgents m_PluginTransmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
    float m_WoundAgentInvasibility;
    override void Init()
    {
        m_TrackActivatedTime = true;
        m_IsPersistent = true;
        m_ID                    = eModifiers.MDF_DISINFECTION;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        //m_WoundAgentInvasibility = m_PluginTransmissionAgents.GetAgentInvasibility(eAgents.WOUND_AGENT);
        DisableActivateCheck();
