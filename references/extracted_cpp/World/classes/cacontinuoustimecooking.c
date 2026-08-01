// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuoustimecooking.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousTimeCooking : CAContinuousBase
{
    const float TIME_DAMAGE_THRESHOLD = 5.0;
 
    protected float             m_LocalTimeElpased;
    protected float             m_TimeElapsed;
    protected float             m_TimeElapsedRepeat;
    protected float             m_TimeElapsedDamage;
    protected ref Param1<float> m_SpentUnits;
    
    protected float             m_TimeToCook;
    protected float             m_AdjustedTimeToCook;
    protected Cooking           m_CookingProcess;
    protected Edible_Base       m_ItemToCook;
    protected float             m_CookingUpdateTime;
    protected float             m_AdjustedCookingUpdateTime;
    
    protected float             m_MinTempToCook;
    
    void CAContinuousTimeCooking(float cooking_update_time)
    {
        m_CookingUpdateTime = cooking_update_time;
