// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionsetalarmclock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSetAlarmClockCB : ActionContinuousBaseCB
{
    const float TIME_TO_COMPLETE = 24.0;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(TIME_TO_COMPLETE);
        AlarmClock_ColorBase alarm = AlarmClock_ColorBase.Cast(m_ActionData.m_MainItem);
        CAContinuousRepeat.Cast(m_ActionData.m_ActionComponent).SetProgress(alarm.GetAnimationPhase(&quot;ClockAlarm&quot;) * TIME_TO_COMPLETE);
