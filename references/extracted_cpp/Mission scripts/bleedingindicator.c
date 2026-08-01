// Source: F:/Games/Dayz/scripts/5_mission/mission/gameplayeffectwidgets/bleedingindicators/bleedingindicator.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class BleedingIndicator extends Managed
{
    protected bool m_Initialized;
    protected bool m_Terminating = false; //doesn&#39;t spawn more drops and ends when the last one does
    protected bool m_EndNow = false;
    protected bool m_IsRunning = false;
    protected int m_DropSpawnsQueued;
    protected int m_ActiveDropsCount;
    protected int m_Severity;
    protected int m_SourceID; //pairs this with the &#39;BleedingSource&#39; bit/ID
    protected GameplayEffectsDataBleeding m_ParentMetaData;
    protected array<float> m_DropProbabilityArray;
    
    protected float m_AverageFrequency; //average drops per interval. NOT changeable on the fly, just a helper value!
    protected float m_SequenceTick;
    protected float m_SequenceDuration;
    protected float m_TimeElapsedTotal;
    protected float m_TimeElapsedSequence;
    protected float m_LastDropSpawnTime; //relative to the TOTAL time, not the sequence!
    protected float m_DropSpawnMinDelay;
    protected float m_DropSpawnMaxDelay;
    protected int m_CurrentDropProbabilityStep;
    protected int m_DropProbabilityRollsCount;
    protected vector m_BasePosition;
    
    ref set<ref BleedingIndicatorDropData> m_ActiveDrops;
    ref set<int> m_CleanupQueue;
    
    void BleedingIndicator(int source_ID, int severity, GameplayEffectsDataBleeding parent)
    {
        m_Initialized = false;
        m_SourceID = source_ID;
        m_Severity = severity;
        m_ParentMetaData = parent;
        m_CurrentDropProbabilityStep = 0;
        m_ActiveDrops = new set<ref BleedingIndicatorDropData>;
        m_CleanupQueue = new set<int>;
        m_DropProbabilityArray = m_ParentMetaData.GetProbabilities(m_Severity);
        m_DropProbabilityRollsCount = m_DropProbabilityArray.Count();
        
        switch (m_Severity)
        {
            case BleedingIndicationConstants.INDICATOR_SEVERITY_LOW:
            {
                m_SequenceDuration = BleedingIndicationConstants.SEQUENCE_DURATION_LOW;
                m_AverageFrequency = BleedingIndicationConstants.SEQUENCE_DROP_AVERAGE_LOW;
                m_DropSpawnMinDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MIN_LOW;
                m_DropSpawnMaxDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MAX_LOW;
                break;
            }
            case BleedingIndicationConstants.INDICATOR_SEVERITY_MEDIUM:
            {
                m_SequenceDuration = BleedingIndicationConstants.SEQUENCE_DURATION_MEDIUM;
                m_AverageFrequency = BleedingIndicationConstants.SEQUENCE_DROP_AVERAGE_MEDIUM;
                m_DropSpawnMinDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MIN_MEDIUM;
                m_DropSpawnMaxDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MAX_MEDIUM;
                break;
            }
            case BleedingIndicationConstants.INDICATOR_SEVERITY_HIGH:
            {
                m_SequenceDuration = BleedingIndicationConstants.SEQUENCE_DURATION_HIGH;
                m_AverageFrequency = BleedingIndicationConstants.SEQUENCE_DROP_AVERAGE_HIGH;
                m_DropSpawnMinDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MIN_HIGH;
                m_DropSpawnMaxDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MAX_HIGH;
                break;
            }
            
            default:
            {
                m_AverageFrequency = BleedingIndicationConstants.SEQUENCE_DROP_AVERAGE_LOW;
                m_DropSpawnMinDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MIN_LOW;
                m_DropSpawnMaxDelay = BleedingIndicationConstants.SEQUENCE_DROP_DELAY_MAX_LOW;
                Debug.Log(&quot;Unknown severity value!&quot;);
            }
#ifdef DIAG_DEVELOPER
            if (DbgBleedingIndicationStaticInfo.m_DbgUseOverrideValues)
            {
                m_SequenceDuration = DbgBleedingIndicationStaticInfo.m_DbgSequenceDuration;
                m_DropSpawnMinDelay = DbgBleedingIndicationStaticInfo.m_DbgDropMinDelay;
                m_DropSpawnMaxDelay = DbgBleedingIndicationStaticInfo.m_DbgDropMaxDelay;
            }
#endif
        }
        
        m_TimeElapsedTotal = 0;
        m_TimeElapsedSequence = 0;
