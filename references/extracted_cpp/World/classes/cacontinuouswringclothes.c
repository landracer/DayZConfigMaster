// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuouswringclothes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousWringClothes : CAContinuousQuantityRepeat
{
    protected float     m_InitItemQuantity;
    protected float     m_SpentQuantityTotal;
    
    void CAContinuousWringClothes( float quantity_used_per_second, float time_to_repeat )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_DefaultTimeToRepeat = time_to_repeat;
        m_InitItemQuantity = -1;
