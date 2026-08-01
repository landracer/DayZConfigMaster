// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondigworms.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDigWormsCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float time_spent;
        time_spent = UATimeSpent.DIG_WORMS;
        if (m_ActionData.m_MainItem.KindOf(&quot;Knife&quot;))
            time_spent = time_spent * 1.2;
        
        if (m_ActionData.m_Player.GetInColdArea())
            time_spent *= GameConstants.COLD_AREA_DIG_WORMS_MODIF;
        
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(time_spent);
