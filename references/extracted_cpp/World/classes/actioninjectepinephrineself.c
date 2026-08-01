// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actioninjectepinephrineself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInjectEpinephrineSelf: ActionInjectSelf
{   
    override void ApplyModifiers( ActionData action_data )
    {
        action_data.m_MainItem.OnApply(action_data.m_Player);
        //action_data.m_Player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_HEART_ATTACK);
        
