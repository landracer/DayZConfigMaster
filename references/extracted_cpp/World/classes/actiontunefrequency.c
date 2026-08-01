// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiontunefrequency.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTuneFrequencyCB : ActionContinuousBaseCB
{
    private const float REPEAT_AFTER_SEC = 1.0;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
