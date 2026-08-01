// Source: F:/Games/Dayz/scripts/4_world/systems/bot/botevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class BotEventBase
{
    PlayerBase m_Player;
    EntityAI m_Entity;
 
    void BotEventBase (PlayerBase p = NULL, EntityAI e = NULL) { m_Entity = e; }
    string DumpToString () {}
