// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/displayelementbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DisplayElementBase extends MessageReceiverBase
{
    int     m_Value;
    int     m_ValueLast = -99999;
    int     m_Key = 0;
    int     m_Type = 0;
    int     m_Range = 0;
    int     m_Colors[7];
    string  m_Labels[7];
    int     m_Pos = 1;
    int     NUM_OF_BITS;
    bool    m_IsClientOnly = false;
    PlayerBase m_Player;
    
    PluginPlayerStatus  m_ModulePlayerStatus;
 
    void DisplayElementBase(PlayerBase player)
    {
        m_System = &quot;VirtualHud&quot;;
        m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
        m_Player = player;
        //SetMaxValue();
