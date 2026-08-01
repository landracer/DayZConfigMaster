// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionbandageself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBandageSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float effectivity       = m_ActionData.m_MainItem.GetBandagingEffectivity();
        float adjustedTimeSpent = 0;
 
        if (effectivity > 0)
            adjustedTimeSpent = UATimeSpent.BANDAGE / effectivity;
 
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(adjustedTimeSpent);
