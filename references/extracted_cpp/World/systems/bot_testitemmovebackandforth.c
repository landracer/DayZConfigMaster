// Source: F:/Games/Dayz/scripts/4_world/systems/bot/bot_testitemmovebackandforth.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BotTestItemMoveBackAndForth extends BotStateBase
{
    EntityAI m_Entity;
    ref BotTestItemMoveBackAndForth_MoveFromSlotToSlot m_Move;
 
    void BotTestItemMoveBackAndForth (Bot bot = NULL, BotStateBase parent = NULL)
    {
        // setup nested state machine
        m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm
 
        m_Move = new BotTestItemMoveBackAndForth_MoveFromSlotToSlot(m_Bot, this);
        
        // events
        BotEventBase __EntAtt__ = new BotEventEntityAttached;
 
        // transitions
        m_FSM.AddTransition(new BotTransition(   m_Move, __EntAtt__, m_Move));
 
        m_FSM.SetInitialState(m_Move);
