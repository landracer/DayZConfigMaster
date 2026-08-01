// Source: F:/Games/Dayz/scripts/4_world/systems/bot/bot_testspawnandopencan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BotEventEntityInHands : BotEventBase { };
 
class BotSpawnEntityInHands extends BotStateBase
{
    string m_Type;
 
    void BotSpawnEntityInHands (Bot bot = NULL, BotStateBase parent = NULL, string new_type = &quot;&quot;)
    {
        if (new_type == string.Empty)
            m_Type = &quot;TunaCan&quot;;
        else
            m_Type = new_type;
