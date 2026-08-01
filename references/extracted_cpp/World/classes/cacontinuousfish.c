// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousfish.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousFish : CAContinuousBase
{
    protected FishingRod_Base m_Rod;
    
    override void Setup( ActionData action_data )
    {
        Class.CastTo(m_Rod, action_data.m_MainItem);
