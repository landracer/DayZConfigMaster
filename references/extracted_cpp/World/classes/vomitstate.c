// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/primary/vomitstate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VomitSymptom : SymptomBase
{
    static const float STAMINA_DEPLETION_MULTIPLIER = 1.3;
    static const float STAMINA_RECOVERY_MULTIPLIER  = 0.5;
 
    //just for the Symptom parameters set-up and gets called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    const int BLOOD_LOSS = 250;
    const int STOMACH_CONTENT_PERCENTAGE_DEFAULT = 5; //per second
    
    private float m_VomitContentPercentage = -1; //considered unspecified at -1
    
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.PRIMARY;
        m_Priority = 100;
        m_ID = SymptomIDs.SYMPTOM_VOMIT;
        m_DestroyOnAnimFinish = true;
        m_SyncToClient = false;
        m_Duration = 5;
        m_MaxCount = 1;
        m_VomitContentPercentage = -1;
