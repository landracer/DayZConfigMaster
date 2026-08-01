// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actioncpr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCPRCB : ActionContinuousBaseCB
{
    private const float REPEAT_AFTER_SEC = 1;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
