// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/casingleusequantity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CASingleUseQuantity : CASingleUseBase
{   
    protected float                 m_QuantityUsedPerAction;
    protected float                 m_ItemQuantity;
    protected ref Param1<float> m_SpentUnits;
 
    void CASingleUseQuantity( float quantity_used_per_action )
    {
        m_QuantityUsedPerAction = quantity_used_per_action; 
