// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncookonstick.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCookOnStickCB : ActionContinuousBaseCB
{
    private const float COOKING_ON_STICK_UPDATE_TIME = 1;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTimeCooking( COOKING_ON_STICK_UPDATE_TIME );
