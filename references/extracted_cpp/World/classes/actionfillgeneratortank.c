// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionfillgeneratortank.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillGeneratorTankCB : ActionContinuousBaseCB
{
    private const float QUANTITY_FILLED_PER_SEC = 400;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFillPowerGenerator(QUANTITY_FILLED_PER_SEC, LIQUID_GASOLINE);
