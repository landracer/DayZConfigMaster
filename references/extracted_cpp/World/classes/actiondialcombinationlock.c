// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondialcombinationlock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDialCombinationLockCB : ActionContinuousBaseCB
{
    private const float REPEAT_AFTER_SEC = 0.5;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
