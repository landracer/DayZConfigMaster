// Source: F:/Games/Dayz/scripts/4_world/systems/bot/bot_stancerandomizer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BotStanceRandomizer extends BotStateBase
{
    ref HumanMovementState m_State = new HumanMovementState();
 
    float m_dtAccumulator = 0.0;
 
    void BotStanceRandomizer(Bot bot = NULL, BotStateBase parent = NULL)
    {
