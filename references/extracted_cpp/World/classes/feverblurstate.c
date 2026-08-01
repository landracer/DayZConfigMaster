// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/feverblurstate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FeverBlurSymptom extends SymptomBase
{
    Material m_MatGauss;
 
    float m_BlurDuration;
    float m_BlurStrength;
    float m_EffectTime;
    float m_EffectStartTime;
    float m_Time;
    protected PPERequester_FeverEffects     m_Requester;
    
    const float BLUR_STRENGTH_MIN = 0.15;
    const float BLUR_STRENGTH_MAX = 0.25;
    
    const int BLUR_DURATION_TIME_MIN = 1.5;
    const int BLUR_DURATION_TIME_MAX = 2.5;
    
    const int MIN_TIME_BETWEEN_EFFECTS = 25.0;
    const int MAX_TIME_BETWEEN_EFFECTS = 35.0;
 
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.SECONDARY;
        m_Priority = 0;
        m_ID = SymptomIDs.SYMPTOM_FEVERBLUR;
        m_DestroyOnAnimFinish = true;
        m_IsPersistent = false;
        m_SyncToClient = true;
        
        if ( !g_Game.IsDedicatedServer() )
        {
            Class.CastTo(m_Requester,PPERequesterBank.GetRequester(PPERequester_FeverEffects));
        }
