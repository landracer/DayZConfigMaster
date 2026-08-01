// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/fsm/states/weaponstartaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class WeaponStartAction extends WeaponStateBase
{
    WeaponActions m_action; 
    int m_actionType; 
 
    void WeaponStartAction (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_action = action;
        m_actionType = actionType;
