// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnoffweaponflashlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffWeaponFlashlight: ActionSingleUseBase
{
    ItemBase m_flashlight;
    
    void ActionTurnOffWeaponFlashlight()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;//CMD_ACTIONMOD_INTERACTONCE
        m_Text = &quot;#switch_off&quot;;
