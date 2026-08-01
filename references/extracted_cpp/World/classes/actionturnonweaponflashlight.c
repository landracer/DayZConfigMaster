// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnonweaponflashlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOnWeaponFlashlight: ActionSingleUseBase
{
    ItemBase m_flashlight;
    
    void ActionTurnOnWeaponFlashlight()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;//CMD_ACTIONMOD_INTERACTONCE
        m_Text = &quot;#switch_on&quot;;
