// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousminewood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousMineWood : CAContinuousBase
{
    protected float                 m_TimeElpased;
    protected float                 m_AdjustedTimeBetweenMaterialDrops;
    protected float                 m_CycleTimeOverride;
    protected float                 m_TimeBetweenMaterialDrops;
    protected float                 m_DamageToMiningItemEachDrop;
    protected float                 m_AdjustedDamageToMiningItemEachDrop;
    protected int                   m_AmountOfDrops;
    protected int                   m_CurrentAssumedDrops;
    protected ref map<string,int>   m_MaterialAndQuantityMap;
    protected float                 m_TimeToComplete;
    protected ref Param1<float>     m_SpentUnits;
    protected bool                  m_DataLoaded = false;
    protected const int             MINEDITEM_MAX = 5;
    protected ItemBase              m_MinedItem[MINEDITEM_MAX];
    protected ItemBase              m_SecondaryItem;
    
    void CAContinuousMineWood(float time_between_drops)
    {
        m_TimeBetweenMaterialDrops = time_between_drops;
        m_CycleTimeOverride = -1.0;
