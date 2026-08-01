// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionviewbinoculars.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionViewBinoculars : ActionViewOptics
{
    bool m_workingRangefinder;
 
    override void OnActionInfoUpdate( PlayerBase player, ActionTarget target, ItemBase item )
    {
        Rangefinder rangefinder = Rangefinder.Cast(item);
        if ( rangefinder )
        {
            m_Text = &quot;#use_range_finder&quot;;
        }
        else
        {
            m_Text = &quot;#Look_Through&quot;;
        }
