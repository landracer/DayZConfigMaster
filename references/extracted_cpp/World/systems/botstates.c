// Source: F:/Games/Dayz/scripts/4_world/systems/bot/botstates.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class BotStateBase
{
    PlayerBase m_Owner; 
    Bot m_Bot; 
    BotStateBase m_ParentState; 
    ref BotFSM m_FSM; 
 
    void BotStateBase (Bot bot = NULL, BotStateBase parent = NULL) { m_Bot = bot; m_Owner = bot.m_Owner; m_ParentState = parent; }
    
    PlayerBase GetPlayerOwner() { return m_Owner; }

    void SetParentState (BotStateBase parent) { m_ParentState = parent; }
    BotStateBase GetParentState () { return m_ParentState; }
 
    bool HasFSM () { return m_FSM != NULL; }
    BotFSM GetFSM () { return m_FSM; }
 
    bool ProcessEvent (BotEventBase e)
    {
        if (HasFSM())
            return m_FSM.ProcessEvent(e);
        return false;
