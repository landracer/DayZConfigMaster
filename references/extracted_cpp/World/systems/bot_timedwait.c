// Source: F:/Games/Dayz/scripts/4_world/systems/bot/bot_timedwait.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class BotEventWaitTimeout : BotEventBase { };
 
class BotTimedWait extends BotStateBase
{
    protected float m_dtAccumulator = 0.0;
    protected float m_Timeout = 3.0;
    protected bool m_Periodic = true;
 
    void BotTimedWait (Bot bot = NULL, BotStateBase parent = NULL, float timeout = 3.0)
    {
        m_Timeout = timeout;
