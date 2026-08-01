// Source: F:/Games/Dayz/scripts/4_world/classes/messagereceiverbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MessageReceiverBase
{
    // message system
    PluginMessageManager m_ModuleMessageManager;
    string m_System;
 
    void OnReceive(int channel) {}
    void OnReceiveInt(int channel, int value) {}
    void OnReceiveFloat(int channel, float value) {}
    void OnReceiveString(int channel, string value) {}
    void OnReceiveParam(int channel, Param params) {}
    
    void MessageReceiverBase()
    {
        m_ModuleMessageManager = PluginMessageManager.Cast(GetPlugin(PluginMessageManager));
