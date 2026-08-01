// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionfillbottlebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
    private int m_liquid_type;
    private float m_BaseFillQuantity;
    
    override void CreateActionComponent()
    {
        m_liquid_type = ActionFillBottleBase.Cast(m_ActionData.m_Action).GetLiquidType(m_ActionData.m_Player, m_ActionData.m_Target, m_ActionData.m_MainItem);
        
        if (m_liquid_type == LIQUID_GASOLINE)
        {
            m_BaseFillQuantity = UAQuantityConsumed.FUEL;
        }
        else if (m_liquid_type == LIQUID_SNOW)
        {
            m_BaseFillQuantity = UAQuantityConsumed.FILL_SNOW;
        }
        else
        {
            m_BaseFillQuantity = UAQuantityConsumed.FILL_LIQUID;
        }
        m_ActionData.m_ActionComponent = new CAContinuousFill(m_BaseFillQuantity, m_liquid_type);
