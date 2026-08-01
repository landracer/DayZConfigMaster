// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handstartaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HandStartAction extends HandStateBase
{
    WeaponActions m_Action; 
    int m_ActionType; 
 
    void HandStartAction (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_Action = action;
        m_ActionType = actionType;
