// Source: F:/Games/Dayz/scripts/3_game/systems/animalcatching/catchingcontextpoissonbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingContextPoissonBase : CatchingContextBase
{
    protected float m_SignalPoissonMean; //usually, only the smallest value is considered
    protected int m_SignalCurrent;
    
    override protected void ClearCatchingItemData()
    {
        super.ClearCatchingItemData();
        
        m_SignalPoissonMean = AnimalCatchingConstants.POISSON_CYCLE_MEAN_DEFAULT;
        m_SignalCurrent = 0;
