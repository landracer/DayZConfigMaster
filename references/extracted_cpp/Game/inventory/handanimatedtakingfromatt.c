// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handanimatedtakingfromatt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandTakingAnimated_Hide extends HandStartAction
{ };
 
class HandTakingAnimated_Show extends HandStartAction
{
    ref InventoryLocation m_Src;
    ref InventoryLocation m_Dst;
 
    void HandTakingAnimated_Show(Man player = null, HandStateBase parent = null, WeaponActions action = WeaponActions.NONE, int actionType = -1)
    {
        m_Src = null;
        m_Dst = null;
