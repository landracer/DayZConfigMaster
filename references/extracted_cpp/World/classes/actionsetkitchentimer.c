// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionsetkitchentimer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSetKitchenTimerCB : ActionContinuousBaseCB
{
    const float TIME_TO_COMPLETE = 24.0;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(TIME_TO_COMPLETE);
        KitchenTimer clock = KitchenTimer.Cast(m_ActionData.m_MainItem);
        CAContinuousRepeat.Cast(m_ActionData.m_ActionComponent).SetProgress(clock.GetAnimationPhase(&quot;ClockAlarm&quot;) * TIME_TO_COMPLETE);
