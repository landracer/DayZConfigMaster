// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/humaninventorywithfsm.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HumanInventoryWithFSM : HumanInventory
{
    protected ref HandFSM m_FSM;  
    protected ref HandStateBase m_Empty;
    protected ref HandStateBase m_Equipped;
 
    void HumanInventoryWithFSM ()
    {
        m_FSM = new HandFSM();
