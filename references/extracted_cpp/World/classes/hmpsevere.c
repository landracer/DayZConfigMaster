// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/hmpsevere.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HMP3Symptom extends SymptomBase
{
    protected float m_EffectDuration;
    protected float m_EffectTime;
    protected float m_EffectStartTime;
    protected float m_Time;
    
    protected PPERequester_HMPGhosts    m_Requester;
    
    const int EFFECT_DURATION_TIME_MIN = 6;
    const int EFFECT_DURATION_TIME_MAX = 10;
    
    const int MIN_TIME_BETWEEN_EFFECTS = 24.0;
    const int MAX_TIME_BETWEEN_EFFECTS = 30.0;
    
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.SECONDARY;
        m_Priority = 0;
        m_ID = SymptomIDs.SYMPTOM_HMP_SEVERE;
        m_IsPersistent = false;
        m_SyncToClient = true;
        m_MaxCount = 1;
        
        if (!g_Game.IsDedicatedServer())
        {
            Class.CastTo(m_Requester,PPERequesterBank.GetRequester(PPERequesterBank.REQ_HMP_GHOST));
        }
