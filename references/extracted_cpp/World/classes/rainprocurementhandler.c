// Source: F:/Games/Dayz/scripts/4_world/classes/rainprocurementhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RainProcurementHandler : Managed
{
    protected MissionBaseWorld m_Mission;
    protected ref set<RainProcurementComponentBase> m_ActiveComponents;
    protected ref set<RainProcurementComponentBase> m_ChangedQueue;
    protected ref set<int> m_CleanupQueue;
    protected const int UPDATE_BATCH_SIZE = 20; //Tweak this to adjust max batch size
    const int UPDATE_TIME = 10; //seconds
    protected bool m_Update;
    protected bool m_ProcessComponents;
    protected bool m_ProcessingFinished;
    protected int m_NextToProcessIdx;
    protected float m_UpdateTimer;
    protected float m_LiquidAmountCoef;
    
    void RainProcurementHandler(MissionBaseWorld mission)
    {
        m_Mission = mission;
        m_Update = false;
        m_ProcessComponents = false;
        m_ProcessingFinished = true;
        m_ActiveComponents = new set<RainProcurementComponentBase>;
        m_ChangedQueue = new set<RainProcurementComponentBase>;
        m_CleanupQueue = new set<int>;
        m_NextToProcessIdx = 0;
        m_UpdateTimer = 0;
