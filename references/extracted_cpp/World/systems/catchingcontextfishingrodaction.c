// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/catchingcontexts/catchingcontextfishingrodaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingContextFishingRodAction : CatchingContextFishingBase
{
    protected PlayerBase m_Player;
    //chances + modifiers
    protected float m_BaitLossChanceMod;
    protected float m_HookLossChanceMod;
    //times
    protected float m_SignalDurationMin; //seconds
    protected float m_SignalDurationMax; //seconds
    protected float m_SignalStartTimeMin; //seconds
    protected float m_SignalStartTimeMax; //seconds
    //signal targets
    protected float m_SignalCycleTarget;
    protected float m_SignalCycleEndTarget;
    protected float m_SignalCycleTargetAdjustment;
    protected float m_SignalCycleTargetEndAdjustment;
    //signal targets - constant
    protected float m_SignalTargetProbability;
    protected float m_SignalTargetEndProbability;
    //misc
    protected int m_SignalCurrent;
    
    //important items
    protected EntityAI m_Hook;
    protected EntityAI m_Bait;
    protected EntityAI m_Rod;
    
    override protected void Init(Param par)
    {
        super.Init(par);
        
        m_Rod = m_MainItem; //only stable one, rest initialized on &#39;InitItemValues&#39; periodically
        m_Player = PlayerBase.Cast(m_MainItem.GetHierarchyRootPlayer());
