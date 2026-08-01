// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actionmanagerserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionManagerServer: ActionManagerBase
{       
    protected ActionBase        m_PendingAction;
    ref ActionReciveData        m_PendingActionReciveData;
    
    void ActionManagerServer(PlayerBase player)
    {
        m_PendingAction             = null;
        m_PendingActionReciveData   = null;
