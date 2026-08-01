// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantityextinguish.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantityExtinguish : CAContinuousQuantityRepeat
{
    private float m_WetnessGainMultiplier = 1.0;
    
    void CAContinuousQuantityExtinguish(float quantity_used_per_second, float time_to_repeat, float wetness_gain_multiplier)
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_DefaultTimeToRepeat   = time_to_repeat;
        m_WetnessGainMultiplier = wetness_gain_multiplier;
