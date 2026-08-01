// Source: F:/Games/Dayz/scripts/4_world/systems/bot/bot_testattachanddropcycle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BotEventEntityAttached : BotEventBase { };
 
class BotEventEntityDetached : BotEventBase { };
 
class BotTestAttachAndDropCycle extends BotStateBase
{
    EntityAI m_Entity;
    ref BotTestAttachAndDropCycle_Detaching m_Detaching;
    ref BotTestAttachAndDropCycle_Attaching m_Attaching;
 
    void BotTestAttachAndDropCycle (Bot bot = NULL, BotStateBase parent = NULL)
    {
        // setup nested state machine
        m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm
 
        m_Detaching = new BotTestAttachAndDropCycle_Detaching(m_Bot, this);
        m_Attaching = new BotTestAttachAndDropCycle_Attaching(m_Bot, this);
        
        // events
        BotEventBase __EntAtt__ = new BotEventEntityAttached;
        BotEventBase __EntDet__ = new BotEventEntityDetached;
 
        // transitions
        m_FSM.AddTransition(new BotTransition(   m_Detaching, __EntDet__, m_Attaching));
        m_FSM.AddTransition(new BotTransition(   m_Attaching, __EntAtt__, m_Detaching));
 
        m_FSM.SetInitialState(m_Detaching);
